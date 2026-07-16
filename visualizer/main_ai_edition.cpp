#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#pragma comment(lib, "ws2_32.lib")

#pragma pack(push, 1)
typedef struct {
    uint16_t C28_Errors;
    uint16_t C28_Errors_Latch;
    uint16_t FPGA_Errors;
    uint16_t FPGA_Errors_Latch;
} Osci_Errors;

typedef struct {
    uint16_t CycleCounter[4];
    Osci_Errors errors;
    uint16_t Current_1;
    uint16_t Current_2;
    uint16_t Voltage_Inp;
    uint16_t Voltage_Out;
    uint16_t FreeTimeCounter;
    uint16_t WatchDog;
    uint16_t __pad[2];
} Osci_Packet;

typedef struct {
    uint16_t cmd;
    uint16_t arg;
} Osci_Request;

typedef struct {
    uint16_t cmd;
    uint16_t len;
    Osci_Errors errors;
} Osci_Response;
#pragma pack(pop)

#define MAX_PLOT_POINTS 1024

char g_ip[32] = "10.1.3.12";
int g_port = 1124;
SOCKET g_sock = INVALID_SOCKET;
bool g_connected = false;
bool g_should_reconnect = false;  // Флаг для запроса переподключения

Osci_Errors g_latest_errors = {0};

Osci_Packet g_packets[MAX_PLOT_POINTS];
int g_packet_count = 0;

float g_plot_c1[MAX_PLOT_POINTS];
float g_plot_c2[MAX_PLOT_POINTS];
float g_plot_v1[MAX_PLOT_POINTS];
float g_plot_v2[MAX_PLOT_POINTS];

CRITICAL_SECTION g_cs;
int g_cmd_request = -1;
int g_arg_request = 0;
bool g_continuous = false;
bool g_running = true;  // Флаг для завершения потока

// Функция для безопасного закрытия сокета
void close_connection() {
    if (g_sock != INVALID_SOCKET) {
        closesocket(g_sock);
        g_sock = INVALID_SOCKET;
    }
    g_connected = false;
}

uint64_t get_cycle_counter(const Osci_Packet* p) {
    uint64_t res = 0;
    res |= (uint64_t)p->CycleCounter[0];
    res |= ((uint64_t)p->CycleCounter[1]) << 16;
    res |= ((uint64_t)p->CycleCounter[2]) << 32;
    res |= ((uint64_t)p->CycleCounter[3]) << 48;
    return res;
}

int cmp_packets(const void* a, const void* b) {
    uint64_t ca = get_cycle_counter((const Osci_Packet*)a);
    uint64_t cb = get_cycle_counter((const Osci_Packet*)b);
    if (ca < cb) return -1;
    if (ca > cb) return 1;
    return 0;
}

bool connect_mcu() {
    // Закрываем предыдущее соединение если есть
    if (g_sock != INVALID_SOCKET) {
        closesocket(g_sock);
        g_sock = INVALID_SOCKET;
    }
    g_connected = false;
    
    g_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (g_sock == INVALID_SOCKET) {
        return false;
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(g_port);
    
    if (inet_pton(AF_INET, g_ip, &addr.sin_addr) != 1) {
        closesocket(g_sock);
        g_sock = INVALID_SOCKET;
        return false;
    }
    
    // Устанавливаем таймаут для подключения
    int timeout = 2000; // 2 секунды
    setsockopt(g_sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(g_sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    
    int opt = 1;
    setsockopt(g_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));

    if (connect(g_sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(g_sock);
        g_sock = INVALID_SOCKET;
        return false;
    }
    
    g_connected = true;
    return true;
}

bool tcp_send_all(const void* buf, int len) {
    if (g_sock == INVALID_SOCKET || !g_connected) return false;
    
    int sent = 0;
    while (sent < len) {
        int res = send(g_sock, (const char*)buf + sent, len - sent, 0);
        if (res <= 0) {
            close_connection();
            return false;
        }
        sent += res;
    }
    return true;
}

bool tcp_recv_all(void* buf, int len) {
    if (g_sock == INVALID_SOCKET || !g_connected) return false;
    
    int rcvd = 0;
    while (rcvd < len) {
        int res = recv(g_sock, (char*)buf + rcvd, len - rcvd, 0);
        if (res <= 0) {
            close_connection();
            return false;
        }
        rcvd += res;
    }
    return true;
}

DWORD WINAPI NetworkThread(LPVOID lpParam) {
    while (g_running) {
        int cmd = -1, arg = 0;
        bool should_connect = false;
        
        EnterCriticalSection(&g_cs);
        if (g_should_reconnect) {
            g_should_reconnect = false;
            should_connect = true;
            cmd = -1;  // Не выполняем команду, только переподключаемся
        } else if (g_cmd_request != -1) {
            cmd = g_cmd_request;
            arg = g_arg_request;
            g_cmd_request = -1;
            should_connect = true;
        } else if (g_continuous) {
            cmd = 2; // PACKET_CMD_FULL
            should_connect = true;
        }
        LeaveCriticalSection(&g_cs);

        if (should_connect) {
            // Пытаемся подключиться если нет активного соединения
            if (g_sock == INVALID_SOCKET || !g_connected) {
                connect_mcu();  // Не блокируем UI, просто пытаемся подключиться
            }
        }

        if (cmd != -1 && g_connected) {
            Osci_Request req = { (uint16_t)cmd, (uint16_t)arg };
            if (tcp_send_all(&req, sizeof(req))) {
                Osci_Response resp;
                if (tcp_recv_all(&resp, sizeof(resp))) {
                    
                    EnterCriticalSection(&g_cs);
                    g_latest_errors = resp.errors;
                    LeaveCriticalSection(&g_cs);

                    if (resp.cmd == 1 || resp.cmd == 2) {
                        int len = resp.len;
                        if (len > 0 && len <= MAX_PLOT_POINTS * (int)sizeof(Osci_Packet)) {
                            void* data = malloc(len);
                            
                            if (data && tcp_recv_all(data, len)) {
                                // Если FULL, читаем tailing INFO packet и offsets
                                if (resp.cmd == 2) {
                                    Osci_Response info_resp;
                                    uint16_t offsets[2];
                                    if (tcp_recv_all(&info_resp, sizeof(info_resp))) {
                                        if (info_resp.len > 0 && info_resp.len <= (int)sizeof(offsets)) {
                                            tcp_recv_all(offsets, info_resp.len);
                                        }
                                        
                                        EnterCriticalSection(&g_cs);
                                        g_latest_errors = info_resp.errors;
                                        LeaveCriticalSection(&g_cs);
                                    }
                                }

                                int count = len / sizeof(Osci_Packet);
                                if (count > MAX_PLOT_POINTS) count = MAX_PLOT_POINTS;

                                EnterCriticalSection(&g_cs);
                                g_packet_count = count;
                                memcpy(g_packets, data, g_packet_count * sizeof(Osci_Packet));
                                
                                // sort by CycleCounter
                                qsort(g_packets, g_packet_count, sizeof(Osci_Packet), cmp_packets);
                                
                                // Prepare float buffers for ImGui PlotLines
                                for (int i = 0; i < g_packet_count; i++) {
                                    g_plot_c1[i] = fabsf((float)g_packets[i].Current_1   - 1706.66f) * 0.366210938f;
                                    g_plot_c2[i] = fabsf((float)g_packets[i].Current_2   - 1706.66f) * 0.366210938f;
                                    g_plot_v1[i] = fabsf((float)g_packets[i].Voltage_Inp - 2070.0f) * 1.271565755f;
                                    g_plot_v2[i] = fabsf((float)g_packets[i].Voltage_Out - 2070.0f) * 1.271565755f;
                                }
                                LeaveCriticalSection(&g_cs);
                            }
                            free(data);
                        }
                    }
                }
            }
        }
        
        Sleep(50);  // Чтобы не спамить постоянно
    }
    return 0;
}

void trigger_cmd(int cmd, int arg) {
    EnterCriticalSection(&g_cs);
    g_cmd_request = cmd;
    g_arg_request = arg;
    LeaveCriticalSection(&g_cs);
}

void trigger_reconnect() {
    EnterCriticalSection(&g_cs);
    g_should_reconnect = true;
    LeaveCriticalSection(&g_cs);
}

int main(int, char**) {
    // Инициализация Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }
    
    InitializeCriticalSection(&g_cs);
    
    // Инициализация GLFW
    if (!glfwInit()) {
        printf("GLFW initialization failed\n");
        WSACleanup();
        return 1;
    }
    
    // Настройка GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    GLFWwindow* window = glfwCreateWindow(800, 800, "MCU TCP Monitor", NULL, NULL);
    if (!window) {
        printf("Window creation failed\n");
        glfwTerminate();
        WSACleanup();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    // Инициализация ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Запуск сетевого потока после инициализации GUI
    CreateThread(NULL, 0, NetworkThread, NULL, 0, NULL);

    static int osci_req_cnt = 16;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        // Проверка на закрытие окна
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Control Panel", NULL, ImGuiWindowFlags_NoCollapse);

        ImGui::Text("Connection Settings");
        ImGui::InputText("IP Address", g_ip, sizeof(g_ip));
        ImGui::InputInt("Port", &g_port);
        
        // Статус подключения
        EnterCriticalSection(&g_cs);
        bool current_connected = g_connected;
        LeaveCriticalSection(&g_cs);
        
        if (current_connected) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Status: CONNECTED");
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Status: DISCONNECTED");
        }
        
        if (ImGui::Button("Connect")) {
            trigger_reconnect();
        }
        
        ImGui::Separator();

        ImGui::Text("Echo Test");
        if (ImGui::Button("Send Echo Packet")) {
            trigger_cmd(0, 0);
        }
        ImGui::Separator();

        ImGui::Text("Request Osci Packets");
        ImGui::InputInt("Packet Count (1-16)", &osci_req_cnt);
        if (osci_req_cnt < 1) osci_req_cnt = 1;
        if (osci_req_cnt > 16) osci_req_cnt = 16;
        if (ImGui::Button("Request Osci")) {
            trigger_cmd(1, osci_req_cnt);
        }
        ImGui::Separator();

        ImGui::Text("Full Request & Graphs");
        if (ImGui::Button("Request Full")) {
            trigger_cmd(2, 0);
        }
        ImGui::SameLine();
        ImGui::Checkbox("Request Continuously", &g_continuous);

        ImGui::Separator();
        
        // Графики
        EnterCriticalSection(&g_cs);
        float w = ImGui::GetContentRegionAvail().x;

        if (g_packet_count > 0) {
            ImGui::PlotLines("##Current 1",   g_plot_c1, g_packet_count, 0, "Current 1",   0.0f, 2000.0f, ImVec2(w, 80));
            ImGui::PlotLines("##Current 2",   g_plot_c2, g_packet_count, 0, "Current 2",   0.0f, 2000.0f, ImVec2(w, 80));
            ImGui::PlotLines("##Voltage In",  g_plot_v1, g_packet_count, 0, "Voltage In",  0.0f, 2000.0f, ImVec2(w, 80));
            ImGui::PlotLines("##Voltage Out", g_plot_v2, g_packet_count, 0, "Voltage Out", 0.0f, 2000.0f, ImVec2(w, 80));
        } else {
            ImGui::Text("No data received yet");
        }
        LeaveCriticalSection(&g_cs);

        ImGui::End();

        // Окно статуса
        ImGui::Begin("Status", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        EnterCriticalSection(&g_cs);
        ImGui::Text("Connection: %s", current_connected ? "ACTIVE" : "INACTIVE");
        ImGui::Separator();
        ImGui::Text("C28_Errors:        0x%04X", g_latest_errors.C28_Errors);
        ImGui::Text("C28_Errors_Latch:  0x%04X", g_latest_errors.C28_Errors_Latch);
        ImGui::Text("FPGA_Errors:       0x%04X", g_latest_errors.FPGA_Errors);
        ImGui::Text("FPGA_Errors_Latch: 0x%04X", g_latest_errors.FPGA_Errors_Latch);
        ImGui::Separator();
        ImGui::Text("Packets in buffer: %d", g_packet_count);
        LeaveCriticalSection(&g_cs);
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Корректное завершение
    g_running = false;
    Sleep(100);  // Даем время потоку завершиться
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    
    close_connection();
    WSACleanup();
    DeleteCriticalSection(&g_cs);
    
    return 0;
}