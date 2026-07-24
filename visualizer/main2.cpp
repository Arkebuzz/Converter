#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

// log
#define LOG_MAX_LINES 67
#define LOG_LINE_LEN 123
CRITICAL_SECTION g_log_cs;
char g_log[LOG_MAX_LINES][LOG_LINE_LEN] = {0};
int g_log_idx = 0;

void app_log(const char* fmt, ...) {
    EnterCriticalSection(&g_log_cs);

    va_list args;
    va_start(args, fmt);
    vsnprintf(g_log[g_log_idx], LOG_LINE_LEN, fmt, args);
    va_end(args);

    g_log_idx = g_log_idx == LOG_MAX_LINES ? 0 : g_log_idx + 1;
    LeaveCriticalSection(&g_log_cs);
}

void app_log_err(const char* msg) {
    static char err_buf[256];
    strerror_s(err_buf, sizeof(err_buf), errno);
    app_log("ERROR %s: %s", msg, err_buf);
}

void glfw_error_callback(int error, const char* desc) {
    app_log("GLFW ERROR %i: %s", error, desc);
}

int tcp_send_all(SOCKET fd, volatile const void *buf, int len) {
    if (len == 0) {
        return 0;
    }
    int bytes_sent = 0;
    while (bytes_sent < len) {
        int sent = send(
            fd,
            ((const char *)buf) + bytes_sent,
            len - bytes_sent,
            0
        );
        if (sent <= 0) {
            app_log_err("tcp_send_all");
            return -1;
        }
        bytes_sent += sent;
    }
    return 0;
}

int tcp_recv_all(SOCKET fd, volatile void *buf, int len) {
    int bytes_read = 0;
    while (bytes_read < len) {
        int read = recv(
            fd,
            ((char *)buf) + bytes_read,
            len - bytes_read,
            0
        );
        if (read <= 0) {
            app_log_err("tcp_recv_all");
            return -1;
        }
        bytes_read += read;
    }
    return 0;
}

typedef uint16_t Uint16;

#pragma pack(1)

typedef struct {
    Uint16 C28_Errors;
    Uint16 C28_Errors_Latch;
    Uint16 FPGA_Errors;
    Uint16 FPGA_Errors_Latch;
} Osci_Errors;

typedef struct {
    Uint16 CycleCounter[4];
    Osci_Errors errors;
    Uint16 Current_1;
    Uint16 Current_2;
    Uint16 Voltage_Inp;
    Uint16 Voltage_Out;
    Uint16 FreeTimeCounter;
    Uint16 WatchDog;
    Uint16 __pad[2];
} Osci_Packet;

typedef struct {
    Uint16 cmd;
    Uint16 arg;
} Osci_Request;

typedef struct {
    Uint16 cmd;
    Uint16 len;
    Osci_Errors errors;
} Osci_Response;

#pragma pack()

typedef enum {
    PACKET_CMD_ECHO = 0,
    PACKET_CMD_OSCI,
    PACKET_CMD_FULL,
    PACKET_CMD_INFO,
} Packet_Cmd;


#define RING_BUF_LEN (2 << 16)

typedef struct {
    Osci_Packet buf[RING_BUF_LEN];
    uint32_t idx;
    CRITICAL_SECTION cs;
} RingBuf;

RingBuf g_rb;

inline void ring_push(const Osci_Packet* packets, uint32_t num_packets) {
    if (num_packets > RING_BUF_LEN) {
        app_log("ERROR: ring_push() num_packets too big (wraps twice)");
    }
    EnterCriticalSection(&g_rb.cs);
    uint32_t num_wrap = num_packets < RING_BUF_LEN - g_rb.idx ? 0 : RING_BUF_LEN - g_rb.idx;
    memcpy(g_rb.buf + g_rb.idx, packets, num_wrap * sizeof(Osci_Packet));
    memcpy(g_rb.buf, packets + num_wrap, (num_packets - num_wrap) * sizeof(Osci_Packet));
    LeaveCriticalSection(&g_rb.cs);
}

int connect_to_mcu(struct in_addr ip, int port, SOCKET *sock) {
    int status = 0;
    SOCKET mcu_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (mcu_sock == INVALID_SOCKET) {
        app_log_err("connect_to_mcu socket()");
        return -1;
    }

    struct sockaddr_in mcu_address = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = ip,
    };

    status = connect(mcu_sock, (struct sockaddr *)&mcu_address, sizeof(mcu_address));
    if (status < 0) {
        app_log_err("connect_to_mcu connect()");
        return -1;
    }

    app_log("CONNECTED");
    *sock = mcu_sock;
    return 0;
}

typedef struct {
    struct in_addr ip;
    uint16_t port;
    uint16_t num_packets;
} SendParams;

DWORD WINAPI send_echo(LPVOID arg) {
    SendParams *params = (SendParams *)arg;
    int status = 0;

    SOCKET sock;
    status = connect_to_mcu(params->ip, params->port, &sock);
    if (status < 0) { return -1; }

    Osci_Request data = { .cmd = PACKET_CMD_ECHO, .arg = 123 };
    status = tcp_send_all(sock, &data, sizeof(data));
    if (status < 0) { return -1; }
    app_log("Sent ECHO");

    Osci_Response resp = {0};
    status = tcp_recv_all(sock, &resp, sizeof(resp));
    if (status < 0) { return -1; }
    
    if (resp.cmd != PACKET_CMD_ECHO) {
        app_log("ERROR: Expected ECHO response, got %u", resp.cmd);
        return -1;
    }

    // print_responce(resp);
    free(params);
    return 0;
}

DWORD WINAPI send_osci(LPVOID arg) {
    SendParams *params = (SendParams *)arg;
    int status = 0;

    SOCKET sock;
    status = connect_to_mcu(params->ip, params->port, &sock);
    if (status < 0) { return -1; }

    Osci_Request data = { .cmd = PACKET_CMD_OSCI, .arg = params->num_packets };
    status = tcp_send_all(sock, &data, sizeof(data));
    if (status < 0) { return -1; }
    app_log("Sent OSCI");

    Osci_Response resp = {0};
    status = tcp_recv_all(sock, &resp, sizeof(resp));
    if (status < 0) { return -1; }
    if (resp.cmd != PACKET_CMD_OSCI ) {
        app_log("ERROR: Expected OSCI response, got %u", resp.cmd);
        return -1;
    }
    // print_responce(resp);
    app_log("Received OSCI response header");

    Osci_Packet *packets = (Osci_Packet *)calloc(resp.len, 1);
    status = tcp_recv_all(sock, packets, resp.len);
    if (status < 0) { return -1; }
    double got_packets = (double)resp.len / sizeof(Osci_Packet);
    app_log(
        "Received OSCI packets: %u (bytes); requested %u packets, got %f",
        resp.len, params->num_packets, got_packets
    );
    ring_push(packets, (uint32_t)got_packets);
    free(packets);
    free(params);
    return 0;
}

DWORD WINAPI send_full(LPVOID arg) {
    SendParams *params = (SendParams *)arg;
    int status = 0;

    SOCKET sock;
    status = connect_to_mcu(params->ip, params->port, &sock);
    if (status < 0) { return -1; }

    Osci_Request data = { .cmd = PACKET_CMD_FULL, .arg = 123 };
    status = tcp_send_all(sock, &data, sizeof(data));
    if (status < 0) { return -1; }
    app_log("Sent OSCI");

    Osci_Response resp = {0};
    status = tcp_recv_all(sock, &resp, sizeof(resp));
    if (status < 0) { return -1; }
    if (resp.cmd != PACKET_CMD_FULL ) {
        app_log("ERROR: Expected FULL response, got %u", resp.cmd);
        return -1;
    }
    // print_responce(resp);
    app_log("Received FULL response header");

    Osci_Packet *packets = (Osci_Packet *)calloc(resp.len, 1);
    status = tcp_recv_all(sock, packets, resp.len);
    double got_packets = (double)resp.len / sizeof(Osci_Packet);
    app_log(
        "Received FULL packets: %u (bytes); %f (packets)\n",
        resp.len, got_packets
    );

    status = tcp_recv_all(sock, &resp, sizeof(resp));
    if (status < 0) { return -1; }
    app_log("Received FULL INFO response header\n");
    if (resp.cmd != PACKET_CMD_INFO ) {
        app_log("ERROR: Expected INFO response, got %u", resp.cmd);
        return -1;
    }

#pragma pack(1)
    struct {
        Uint16 before_offset;
        Uint16 after_offset;
    } info_data = {0};
#pragma pack()
    if (resp.len != sizeof(info_data)) {
        app_log("ERROR: Mismatched INFO header response length", resp.cmd);
        return -1;
    }
    tcp_recv_all(sock, &info_data, sizeof(info_data));

    app_log(
        "Received FULL INFO response body: "
        "before_offset = %u; after_offset = %u\n",
        info_data.before_offset, info_data.after_offset
    );

    ring_push(packets, (uint32_t)got_packets);
    free(packets);
    free(params);
    return 0;
}

int main(void) {
    InitializeCriticalSection(&g_log_cs);
    InitializeCriticalSection(&g_rb.cs);

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(800, 600, "TCP Visualizer", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    struct in_addr ip;
    char ip_str[32] = "10.1.3.12";
    uint16_t port = 1124;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // monitor window
        {
            ImGui::Begin("Monitor");

            ImGui::InputText("IP", ip_str, sizeof(ip_str));
            inet_pton(AF_INET, ip_str, &ip);
            ImGui::InputScalar("Port", ImGuiDataType_U16, &port);
            ImGui::Separator();

            if (ImGui::Button("Send ECHO")) {
                SendParams *params = (SendParams *)malloc(sizeof(SendParams));
                *params = { .ip = ip, .port = port };
                CreateThread(NULL, 0, send_echo, params, NULL, 0);
            }
            ImGui::SameLine();
            ImGui::Text("Sent");

            ImGui::End();
        }

        // log window
        {
            ImGui::Begin("Log");
            EnterCriticalSection(&g_log_cs);
            for (int i = 0; i < g_log_idx; i++) {
                ImGui::TextUnformatted(g_log[i]);
            }
            for (int i = g_log_idx; i < LOG_MAX_LINES; i++) {
                ImGui::TextUnformatted(g_log[i]);
            }
            LeaveCriticalSection(&g_log_cs);
            ImGui::End();
        }

        // rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    return 0;
}

