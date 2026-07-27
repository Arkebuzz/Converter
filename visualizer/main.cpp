#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <stdio.h>
#include <stdint.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "GLFW/glfw3.h"

// log
#define LOG_MAX_LINES 67
#define LOG_LINE_LEN 420
CRITICAL_SECTION g_log_cs;
char g_log[LOG_MAX_LINES][LOG_LINE_LEN] = {0};
int g_log_idx = 0;

void app_log(const char* fmt, ...) {
    EnterCriticalSection(&g_log_cs);

    va_list args;
    va_start(args, fmt);
    vsnprintf(g_log[g_log_idx], LOG_LINE_LEN, fmt, args);
    va_end(args);

    g_log_idx = (g_log_idx + 1) % LOG_MAX_LINES;
    LeaveCriticalSection(&g_log_cs);
}

void app_log_err(const char* msg) {
    static char err_buf[256];
    int err = WSAGetLastError();
    DWORD res = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        err_buf,
        sizeof(err_buf),
        NULL
    );
    if (res == 0) {
        app_log("ERROR %s: (unknown WSA error %u)", msg, err);
        return;
    }
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

// ring buffer for packets
typedef struct {
    Osci_Packet buf[RING_BUF_LEN];
    uint32_t idx;
    CRITICAL_SECTION cs;
} RingBuf;

RingBuf g_rb;

inline void ring_push(const Osci_Packet* packets, uint32_t num_packets) {
    if (num_packets > RING_BUF_LEN) {
        app_log("ERROR: ring_push() num_packets too big (wraps twice)");
        return;
    }
    EnterCriticalSection(&g_rb.cs);
    uint32_t num_wrap = num_packets < RING_BUF_LEN - g_rb.idx ? 0 : RING_BUF_LEN - g_rb.idx;
    memcpy(g_rb.buf + g_rb.idx, packets, (num_packets - num_wrap) * sizeof(Osci_Packet));
    memcpy(g_rb.buf, packets + num_wrap, num_wrap * sizeof(Osci_Packet));
    g_rb.idx = (g_rb.idx + num_packets) % RING_BUF_LEN;
    LeaveCriticalSection(&g_rb.cs);
}

// double buffering for last error
typedef struct {
    struct {
        Osci_Errors errors;
        uint16_t cmd;
    } buf[2];
    uint8_t active_idx;
    CRITICAL_SECTION cs;
} ErrorsBuf;

ErrorsBuf g_eb;

// NOTE: UI doesn't use critical section - it just reads the active index
// so as long as only one thread sets the errors critical section is almost zero cost
inline void errors_set(Osci_Errors errors, uint16_t cmd) {
    EnterCriticalSection(&g_eb.cs);
    uint8_t inactive_idx = g_eb.active_idx ^ 1;
    g_eb.buf[inactive_idx] = { errors, cmd };
    g_eb.active_idx = inactive_idx;
    LeaveCriticalSection(&g_eb.cs);
}

int connect_to_mcu(const char *ip, int port, SOCKET *sock) {
    int status = 0;
    SOCKET mcu_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (mcu_sock == INVALID_SOCKET) {
        app_log_err("connect_to_mcu socket()");
        return -1;
    }

    struct sockaddr_in mcu_address = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    status = inet_pton(AF_INET, ip, &mcu_address.sin_addr);
    if (status <= 0) {
        if (status == 0) {
            app_log("ERROR: Incorrent IP format: %s", ip);
        } else {
            app_log_err("connect_to_mcu inet_pton()");
        }
        return -1;
    }

    int ndelay = 1;
    setsockopt(mcu_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&ndelay, sizeof(ndelay));

    status = connect(mcu_sock, (struct sockaddr *)&mcu_address, sizeof(mcu_address));
    if (status < 0) {
        app_log_err("connect_to_mcu connect()");
        return -1;
    }

    app_log("CONNECTED to %s:%u", ip, port);
    *sock = mcu_sock;
    return 0;
}

typedef struct {
    char ip[32];
    uint16_t port;
    uint16_t num_packets;
} SendParams;

SendParams *send_params_new(uint16_t port, char *ip) {
    SendParams *params = (SendParams *)malloc(sizeof(SendParams));
    params->port = port;
    memcpy(&params->ip, ip, sizeof(params->ip));
    return params;
}

bool g_repeat = false;

const char *cmd_to_str(uint16_t cmd) {
#define CMD_CASE_RET(cmd) case cmd: return #cmd
    switch (cmd) {
        CMD_CASE_RET(PACKET_CMD_ECHO);
        CMD_CASE_RET(PACKET_CMD_OSCI);
        CMD_CASE_RET(PACKET_CMD_FULL);
        CMD_CASE_RET(PACKET_CMD_INFO);
        default: return "(UNKNOWN CMD)";
    }
}

int recv_packet_header(SOCKET sock, Osci_Request req, Osci_Response *resp) {
    int status = 0;
    status = tcp_send_all(sock, &req, sizeof(req));
    if (status < 0) { return -1; }
    app_log("Sent %s", cmd_to_str(req.cmd));

    status = tcp_recv_all(sock, resp, sizeof(*resp));
    if (status < 0) { return -1; }
    
    if (resp->cmd != req.cmd) {
        app_log("ERROR: Expected %s response, got %u", cmd_to_str(req.cmd), resp->cmd);
        return -1;
    }
    errors_set(resp->errors, req.cmd);
    return 0;
}

DWORD WINAPI send_echo(LPVOID arg) {
    int ret = 0;
    int status = 0;
    SendParams *params = (SendParams *)arg;
    Osci_Response resp;

    SOCKET sock;
    status = connect_to_mcu(params->ip, params->port, &sock);
    if (status < 0) { goto err; }

    status = recv_packet_header(
        sock,
        Osci_Request { .cmd = PACKET_CMD_ECHO, .arg = 123 },
        &resp
    );
    if (status < 0) { goto err; }

    if (0) err: {
        ret = -1;
    }
    free(params);
    closesocket(sock);
    return ret;
}

DWORD WINAPI send_osci(LPVOID arg) {
    int ret = 0;
    int status = 0;
    SendParams *params = (SendParams *)arg;
    Osci_Response resp;

    SOCKET sock;
    status = connect_to_mcu(params->ip, params->port, &sock);
    if (status < 0) { goto err; }

    status = recv_packet_header(
        sock,
        Osci_Request { .cmd = PACKET_CMD_OSCI, .arg = params->num_packets },
        &resp
    );
    if (status < 0) { goto err; }

    {
        Osci_Packet *packets = (Osci_Packet *)calloc(resp.len, 1);
        status = tcp_recv_all(sock, packets, resp.len);
        if (status < 0) { goto err; }
        double got_packets = (double)resp.len / sizeof(Osci_Packet);
        app_log(
            "Received OSCI packets: %u (bytes); requested %u packets, got %f",
            resp.len, params->num_packets, got_packets
        );
        ring_push(packets, (uint32_t)got_packets);
        free(packets);
    }

    if (0) err: {
        ret = -1;
    }
    free(params);
    closesocket(sock);
    return ret;
}

DWORD WINAPI send_full(LPVOID arg) {
    int ret = 0;
    int status = 0;
    SendParams *params = (SendParams *)arg;

    SOCKET sock;
    status = connect_to_mcu(params->ip, params->port, &sock);
    if (status < 0) { goto err; }

    do {
        Osci_Response resp; // forward declare because bjarne is a moron
        Osci_Packet *packets;
        double got_packets;

        status = recv_packet_header(
            sock,
            Osci_Request { .cmd = PACKET_CMD_FULL, .arg = 123 },
            &resp
        );
        if (status < 0) { goto err; }

        packets = (Osci_Packet *)calloc(resp.len, 1);
        status = tcp_recv_all(sock, packets, resp.len);
        if (status < 0) { goto err; }
        got_packets = (double)resp.len / sizeof(Osci_Packet);
        app_log(
            "Received FULL packets: %u (bytes); %f (packets)\n",
            resp.len, got_packets
        );

        status = recv_packet_header(
            sock,
            Osci_Request { .cmd = PACKET_CMD_INFO, .arg = 123 },
            &resp
        );
        if (status < 0) { goto err; }

#pragma pack(1)
        struct {
            Uint16 before_offset;
            Uint16 after_offset;
        } info_data = {0};
#pragma pack()
        if (resp.len != sizeof(info_data)) {
            app_log("ERROR: Mismatched INFO header response length");
            goto err;
        }
        tcp_recv_all(sock, &info_data, sizeof(info_data));

        app_log(
            "Received FULL INFO response body: "
            "before_offset = %u; after_offset = %u\n",
            info_data.before_offset, info_data.after_offset
        );
        ring_push(packets, (uint32_t)got_packets);

        free(packets);
    } while (g_repeat);

    if (0) err: {
        ret = -1;
    }
    free(params);
    closesocket(sock);
    return ret;
}

int main(void) {
    InitializeCriticalSection(&g_log_cs);
    InitializeCriticalSection(&g_rb.cs);
    InitializeCriticalSection(&g_eb.cs);

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(800, 600, "TCP Visualizer", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    char ip[32] = "10.1.3.12";
    uint16_t port = 1124;
    uint16_t num_packets = 16;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // monitor window
        {
            ImGui::Begin("Monitor");

            ImGui::InputText("IP", ip, sizeof(ip));
            ImGui::InputScalar("Port", ImGuiDataType_U16, &port);
            ImGui::Separator();

            if (ImGui::Button("Send ECHO")) {
                SendParams *params = send_params_new(port, ip);
                HANDLE thr = CreateThread(NULL, 0, send_echo, params, NULL, 0);
                if (thr) { CloseHandle(thr); }
            }

            if (ImGui::Button("Send OSCI")) {
                SendParams *params = send_params_new(port, ip);
                params->num_packets = num_packets;
                HANDLE thr = CreateThread(NULL, 0, send_osci, params, NULL, 0);
                if (thr) { CloseHandle(thr); }
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(20.0f);
            ImGui::InputScalar("Num Packets", ImGuiDataType_U16, &num_packets);

            if (ImGui::Button("Send FULL")) {
                SendParams *params = send_params_new(port, ip);
                HANDLE thr = CreateThread(NULL, 0, send_full, params, NULL, 0);
                if (thr) { CloseHandle(thr); }
            }
            ImGui::SameLine();
            ImGui::Checkbox("Repeat", &g_repeat);

            ImGui::End();
        }

        // errors window
        {
            ImGui::Begin("Errors");
            int idx = g_eb.active_idx;
            int cmd = g_eb.buf[idx].cmd;
            Osci_Errors errors = g_eb.buf[idx].errors;
            ImGui::Text(
                "Cmd: %s\n"
                "Osci_Errors.C28_Errors        = %u\n"
                "Osci_Errors.C28_Errors_Latch  = %u\n"
                "Osci_Errors.FPGA_Errors       = %u\n"
                "Osci_Errors.FPGA_Errors_Latch = %u\n",
                cmd_to_str(cmd),
                errors.C28_Errors, errors.C28_Errors_Latch,
                errors.FPGA_Errors, errors.FPGA_Errors_Latch
            );
            ImGui::End();
        }

        // log window
        {
            ImGui::Begin("Log");
            EnterCriticalSection(&g_log_cs);
            for (int i = 0; i < g_log_idx; i++) {
                ImGui::TextWrapped(g_log[i]);
            }
            for (int i = g_log_idx; i < LOG_MAX_LINES; i++) {
                ImGui::TextWrapped(g_log[i]);
            }
            LeaveCriticalSection(&g_log_cs);
            ImGui::End();
        }

        // plots
        {
            ImGui::Begin("Plots");
            if (ImPlot::BeginPlot("Oscilloscope Data", ImVec2(-1, -1))) {
                ImPlot::SetupAxes("Time (CycleCounter)", "Value");
                
                static Osci_Packet local_buf[RING_BUF_LEN];
                static uint64_t time_vals[RING_BUF_LEN];
                static uint64_t current_1_vals[RING_BUF_LEN];
                static uint64_t current_2_vals[RING_BUF_LEN];
                static uint64_t voltage_inp_vals[RING_BUF_LEN];
                static uint64_t voltage_out_vals[RING_BUF_LEN];
                
                EnterCriticalSection(&g_rb.cs);
                uint32_t oldest_idx = g_rb.idx;
                uint32_t first_chunk = RING_BUF_LEN - oldest_idx;
                
                memcpy(local_buf, g_rb.buf + oldest_idx, first_chunk * sizeof(Osci_Packet));
                if (oldest_idx > 0) {
                    memcpy(local_buf + first_chunk, g_rb.buf, oldest_idx * sizeof(Osci_Packet));
                }
                LeaveCriticalSection(&g_rb.cs);

                for (uint32_t i = 0; i < RING_BUF_LEN; i++) {
                    time_vals[i] = (uint64_t)local_buf[i].CycleCounter[3] << 48
                                 | (uint64_t)local_buf[i].CycleCounter[2] << 32
                                 | (uint64_t)local_buf[i].CycleCounter[1] << 16
                                 | (uint64_t)local_buf[i].CycleCounter[0];
                                        
                    current_1_vals[i]   = (uint64_t)local_buf[i].Current_1;
                    current_2_vals[i]   = (uint64_t)local_buf[i].Current_2;
                    voltage_inp_vals[i] = (uint64_t)local_buf[i].Voltage_Inp;
                    voltage_out_vals[i] = (uint64_t)local_buf[i].Voltage_Out;
                }

                ImPlot::PlotLine("Current_1",   time_vals, current_1_vals,   RING_BUF_LEN);
                ImPlot::PlotLine("Current_2",   time_vals, current_2_vals,   RING_BUF_LEN);
                ImPlot::PlotLine("Voltage_Inp", time_vals, voltage_inp_vals, RING_BUF_LEN);
                ImPlot::PlotLine("Voltage_Out", time_vals, voltage_out_vals, RING_BUF_LEN);
                
                ImPlot::EndPlot();
            }
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

