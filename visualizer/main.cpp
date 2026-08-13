// FIXME: massive memory leak somewhere

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

// debug
#if !defined NDEBUG
uint64_t alloc_cntr = 0; uint64_t free_cntr = 0;
void *spoof_malloc(size_t a) {
    alloc_cntr++;
    return malloc(a);
}
void *spoof_calloc(size_t a, size_t b) {
    alloc_cntr++;
    return calloc(a,b);
}
void spoof_free(void *arg) {
    free_cntr++;
    free(arg);
}
#define malloc(a) spoof_malloc(a)
#define calloc(a, b) spoof_calloc(a, b)
#define free(a) spoof_free(a)
#endif

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
extern "C" {
#include "communication_data.h"
}
#pragma pack()


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

    uint32_t space_left = RING_BUF_LEN - g_rb.idx;
    if (num_packets <= space_left) {
        memcpy(g_rb.buf + g_rb.idx, packets, num_packets * sizeof(Osci_Packet));
    } else {
        uint32_t wrap_count = num_packets - space_left;
        memcpy(g_rb.buf + g_rb.idx, packets, space_left * sizeof(Osci_Packet));
        memcpy(g_rb.buf, packets + space_left, wrap_count * sizeof(Osci_Packet));
    }

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

// wow c++ is garbage
// thanks for breaking C compatibility
using enum Flash_Data::Flash_Cmd;
typedef Flash_Data::Flash_Cmd Flash_Cmd;

typedef struct {
    char ip[32];
    uint16_t port;
    uint16_t num_packets;
    struct {
        Flash_Cmd cmd;
        uint32_t address;
        uint16_t data_size;
    } flash_params;
} SendParams;

SendParams *send_params_new(uint16_t port, char *ip) {
    SendParams *params = (SendParams *)malloc(sizeof(SendParams));
    params->port = port;
    memcpy(&params->ip, ip, sizeof(params->ip));
    return params;
}

bool g_repeat = false;

#define ENUM_CASE_RET(cmd) case cmd: return #cmd
const char *cmd_to_str(uint16_t cmd) {
    switch (cmd) {
        ENUM_CASE_RET(PACKET_CMD_ECHO);
        ENUM_CASE_RET(PACKET_CMD_OSCI);
        ENUM_CASE_RET(PACKET_CMD_FULL);
        ENUM_CASE_RET(PACKET_CMD_INFO);
        ENUM_CASE_RET(PACKET_CMD_FLASH);
        default: return "(UNKNOWN CMD)";
    }
}

const char *flash_cmd_to_str(uint16_t cmd) {
    switch (cmd) {
        ENUM_CASE_RET(FLASH_CMD_DONE);
        ENUM_CASE_RET(FLASH_CMD_BUSY);
        ENUM_CASE_RET(FLASH_CMD_READ);
        ENUM_CASE_RET(FLASH_CMD_WRITE);
        ENUM_CASE_RET(FLASH_CMD_ERASE_4K);
        ENUM_CASE_RET(FLASH_CMD_SZ);
        default: return "(UNKNOWN FLASH CMD)";
    }
}

int send_request(SOCKET sock, Osci_Request req) {
    int status = 0;
    status = tcp_send_all(sock, &req, sizeof(req));
    if (status < 0) { return -1; }
    app_log("Sent %s", cmd_to_str(req.cmd));

    return 0;
}

int recv_header(SOCKET sock, Osci_Response *resp) {
    int status = 0;
    status = tcp_recv_all(sock, resp, sizeof(*resp));
    if (status < 0) { return -1; }
    
    return 0;
}

int send_and_recv_header(SOCKET sock, Osci_Request req, Osci_Response *resp) {
    if (send_request(sock, req) < 0) { return -1; }
    if (recv_header(sock, resp) < 0) { return -1; }
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

    status = send_and_recv_header(
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

    status = send_and_recv_header(
        sock,
        Osci_Request { .cmd = PACKET_CMD_OSCI, .arg = params->num_packets },
        &resp
    );
    if (status < 0) { goto err; }

    {
        Osci_Packet *packets = (Osci_Packet *)calloc(resp.len, 1);
        status = tcp_recv_all(sock, packets, resp.len);
        if (status < 0) { free(packets); goto err; }
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

        status = send_and_recv_header(
            sock,
            Osci_Request { .cmd = PACKET_CMD_FULL, .arg = 123 },
            &resp
        );
        if (status < 0) { goto err; }

        packets = (Osci_Packet *)calloc(resp.len, 1);
        status = tcp_recv_all(sock, packets, resp.len);
        if (status < 0) { free(packets); goto err; }
        got_packets = (double)resp.len / sizeof(Osci_Packet);
        app_log(
            "Received FULL packets: %u (bytes); %f (packets)\n",
            resp.len, got_packets
        );

        status = send_and_recv_header(
            sock,
            Osci_Request { .cmd = PACKET_CMD_INFO, .arg = 123 },
            &resp
        );
        if (status < 0) { free(packets); goto err; }

#pragma pack(1)
        struct {
            Uint16 before_offset;
            Uint16 after_offset;
        } info_data = {0};
#pragma pack()
        if (resp.len != sizeof(info_data)) {
            app_log("ERROR: Mismatched INFO header response length");
            free(packets);
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

#define FLASH_DATA_LEN (sizeof(Flash_Data::Buf))

uint16_t g_flash_cmd = {0};
char g_flash_read_data[FLASH_DATA_LEN] = {0};
char g_flash_write_data[FLASH_DATA_LEN] = {0};
static_assert(sizeof(char) == 1);

CRITICAL_SECTION g_flash_read_cs;
CRITICAL_SECTION g_flash_write_cs;

DWORD WINAPI send_flash(LPVOID arg) {
    int ret = 0;
    int status = 0;
    SendParams *params = (SendParams *)arg;
    Osci_Response resp;

    // Flash_Cmd cmd = params->flash_params.cmd;
    // uint32_t address = params->flash_params.address;
    // uint16_t data_size = params->flash_params.data_size;

    Flash_Cmd cmd = params->flash_params.cmd;
    uint32_t address = 0xdeadbabe;
    uint16_t data_size = 0x1234;

    uint16_t is_ready = 0;

    // it doesn't work if I declare it via designated initializers
    // does anything fucking work in this language
    Osci_Request req = {0};
    req.cmd = PACKET_CMD_FLASH;
    req.arg = cmd;

    SOCKET sock;
    status = connect_to_mcu(params->ip, params->port, &sock);
    if (status < 0) { goto err; }

    status = send_and_recv_header(sock, req, &resp);
    if (status < 0) { goto err; }

    app_log("RESP RECV'D");

    if (resp.len != sizeof(is_ready)) {
        app_log("FLASH ERROR: resp.len != sizeof(is_ready)");
        goto err;
    }

    status = tcp_recv_all(sock, &is_ready, resp.len); 
    if (status < 0) { goto err; }

    app_log("READY RECV'D");

    if (!is_ready) {
        app_log("NOT READY");

        InterlockedExchange16((SHORT *)&g_flash_cmd, FLASH_CMD_BUSY);
        free(params);
        closesocket(sock);
        return 0;
    }

    Sleep(10);
    status = tcp_send_all(sock, &address, sizeof(address));
    if (status < 0) { goto err; }

    app_log("ADDRESS SENT");

    if (cmd == FLASH_CMD_WRITE || cmd == FLASH_CMD_READ) {
        // status = tcp_send_all(sock, &data_size, sizeof(data_size));
        app_log("DATA SZ SENT");
        if (status < 0) { goto err; }
    }
    if (cmd == FLASH_CMD_WRITE) {
        EnterCriticalSection(&g_flash_write_cs);
        // status = tcp_send_all(sock, &g_flash_write_data, data_size);
        LeaveCriticalSection(&g_flash_write_cs);
        app_log("DATA SENT");
        if (status < 0) { goto err; }
    }

    InterlockedExchange16((SHORT *)&g_flash_cmd, cmd);

    status = recv_header(sock, &resp);
    if (status < 0) { goto err; }
    app_log("HEADER RECV'D");

    if (cmd == FLASH_CMD_READ) {
        EnterCriticalSection(&g_flash_read_cs);
        status = tcp_recv_all(sock, g_flash_read_data, resp.len);
        LeaveCriticalSection(&g_flash_read_cs);
        app_log("DATA RECV'D");
        if (status < 0) { goto err; }
        if (resp.len != data_size) {
            app_log("FLASH ERROR: resp.len != data_size");
        }
    }

    if (0) err: {
        app_log("ERR'D");
        ret = -1;
    }
    free(params);
    closesocket(sock);
    return ret;
}

int main(void) {
    InitializeCriticalSection(&g_flash_read_cs);
    InitializeCriticalSection(&g_flash_write_cs);
    InitializeCriticalSection(&g_log_cs);
    InitializeCriticalSection(&g_rb.cs);
    InitializeCriticalSection(&g_eb.cs);

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(1000, 700, "TCP Visualizer", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static char ip[32] = "10.1.3.12";
        static uint16_t port = 1124;
        static uint16_t num_packets = 16;

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

        // flash window
        {
            ImGui::Begin("Flash");

            ImGui::Text("Flash Response Status: %s", flash_cmd_to_str(g_flash_cmd));

            static Flash_Cmd flash_cmd = FLASH_CMD_READ;
            if (ImGui::RadioButton("Read", flash_cmd == FLASH_CMD_READ)) {
                flash_cmd = FLASH_CMD_READ;
            }
            if (ImGui::RadioButton("Write", flash_cmd == FLASH_CMD_WRITE)) {
                flash_cmd = FLASH_CMD_WRITE;
            }
            if (ImGui::RadioButton("Erase 4K", flash_cmd == FLASH_CMD_ERASE_4K)) {
                flash_cmd = FLASH_CMD_ERASE_4K;
            }

            static uint32_t address = 0;
            static uint8_t data_size = 0;
            ImGui::InputScalar("Address", ImGuiDataType_U32, &address,
                    NULL, NULL, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
            if (flash_cmd == FLASH_CMD_READ) {
                EnterCriticalSection(&g_flash_read_cs);

                ImGui::InputScalar("Data Size", ImGuiDataType_U8, &data_size);
                ImGui::Text("Data");
                char prev_char = g_flash_read_data[data_size];
                g_flash_read_data[data_size] = '\0';
                ImGui::InputTextMultiline("##ReadData", g_flash_read_data, data_size,
                        ImVec2(-1.0f, 150.0f), ImGuiInputTextFlags_ReadOnly);
                g_flash_read_data[data_size] = prev_char;

                LeaveCriticalSection(&g_flash_read_cs);
            } else if (flash_cmd == FLASH_CMD_WRITE) {
                EnterCriticalSection(&g_flash_write_cs);

                ImGui::Text("Data");
                ImGui::InputTextMultiline("##WriteData", g_flash_write_data,
                        FLASH_DATA_LEN, ImVec2(-1.0f, 150.0f));

                LeaveCriticalSection(&g_flash_write_cs);
            }

            if (ImGui::Button("Send FLASH")) {
                SendParams *params = send_params_new(port, ip);
                params->flash_params.cmd = flash_cmd;
                params->flash_params.address = address;
                params->flash_params.data_size = data_size;
                HANDLE thr = CreateThread(NULL, 0, send_flash, params, NULL, 0);
                if (thr) { CloseHandle(thr); }
            }

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
            
            static float history = 1000000.0f;
            ImGui::SliderFloat("History (Cycles)", &history, 1000.0f, 100000000.0f, "%.0f", ImGuiSliderFlags_Logarithmic);

            static bool has_wrapped = false;
            static uint32_t last_idx = 0;

            EnterCriticalSection(&g_rb.cs);
            uint32_t current_idx = g_rb.idx;
            LeaveCriticalSection(&g_rb.cs);

            if (current_idx < last_idx) {
                has_wrapped = true;
            }
            last_idx = current_idx;

            uint32_t valid_count = has_wrapped ? RING_BUF_LEN : current_idx;

            #define MAX_RENDER_POINTS 8192 
            static Osci_Packet local_packets[MAX_RENDER_POINTS];
            static double time_vals[MAX_RENDER_POINTS];
            static double current1_vals[MAX_RENDER_POINTS];
            static double current2_vals[MAX_RENDER_POINTS];
            static double volt_in_vals[MAX_RENDER_POINTS];
            static double volt_out_vals[MAX_RENDER_POINTS];
            
            int display_count = 0;

            if (valid_count > 0) {
                float plot_width = ImGui::GetContentRegionAvail().x;
                int max_points = (plot_width > 0) ? (int)(plot_width * 2) : 2000;
                
                int stride = valid_count / max_points;
                if (stride < 1) stride = 1;
                display_count = valid_count / stride;

                if (display_count > MAX_RENDER_POINTS) display_count = MAX_RENDER_POINTS;
                
                uint32_t oldest_idx = has_wrapped ? current_idx : 0;

                EnterCriticalSection(&g_rb.cs);
                for (int i = 0; i < display_count; i++) {
                    uint32_t buf_idx = (oldest_idx + (i * stride)) % RING_BUF_LEN;
                    local_packets[i] = g_rb.buf[buf_idx];
                }
                LeaveCriticalSection(&g_rb.cs);

                for (int i = 0; i < display_count; i++) {
                    Osci_Packet& p = local_packets[i];
                    
                    uint64_t t = ((uint64_t)p.CycleCounter[3] << 48) |
                                 ((uint64_t)p.CycleCounter[2] << 32) |
                                 ((uint64_t)p.CycleCounter[1] << 16) |
                                 ((uint64_t)p.CycleCounter[0]);
                                 
                    time_vals[i]     = (double)t;
                    current1_vals[i] = (double)p.Current_1;
                    current2_vals[i] = (double)p.Current_2;
                    volt_in_vals[i]  = (double)p.Voltage_Inp;
                    volt_out_vals[i] = (double)p.Voltage_Out;
                }
            }

            double latest_t = (valid_count > 0 && display_count > 0) ? time_vals[display_count - 1] : history;

            const ImPlotAxisFlags x_flags = ImPlotAxisFlags_None;
            const ImPlotAxisFlags y_flags = ImPlotAxisFlags_AutoFit;

            if (ImPlot::BeginSubplots("##OsciSubplots", 4, 1, ImVec2(-1, -1), ImPlotSubplotFlags_LinkAllX)) {
                
                if (ImPlot::BeginPlot("##C1")) {
                    ImPlot::SetupAxes(nullptr, "Current 1", x_flags, y_flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, latest_t - history, latest_t, ImGuiCond_Always);
                    if (valid_count > 0) ImPlot::PlotLine("##C1_line", time_vals, current1_vals, display_count);
                    ImPlot::EndPlot();
                }

                if (ImPlot::BeginPlot("##C2")) {
                    ImPlot::SetupAxes(nullptr, "Current 2", x_flags, y_flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, latest_t - history, latest_t, ImGuiCond_Always);
                    if (valid_count > 0) ImPlot::PlotLine("##C2_line", time_vals, current2_vals, display_count);
                    ImPlot::EndPlot();
                }

                if (ImPlot::BeginPlot("##Vin")) {
                    ImPlot::SetupAxes(nullptr, "Voltage In", x_flags, y_flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, latest_t - history, latest_t, ImGuiCond_Always);
                    if (valid_count > 0) ImPlot::PlotLine("##Vin_line", time_vals, volt_in_vals, display_count);
                    ImPlot::EndPlot();
                }

                if (ImPlot::BeginPlot("##Vout")) {
                    ImPlot::SetupAxes("Time (CycleCounter)", "Voltage Out", x_flags, y_flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, latest_t - history, latest_t, ImGuiCond_Always);
                    if (valid_count > 0) ImPlot::PlotLine("##Vout_line", time_vals, volt_out_vals, display_count);
                    ImPlot::EndPlot();
                }

                ImPlot::EndSubplots();
            }

#if !defined NDEBUG
            // debug
            {
                ImGui::Begin("Debug");
                ImGui::Text("alloc_cntr: %zu", alloc_cntr);
                ImGui::Text("free_cntr: %zu", free_cntr);
                ImGui::End();
            }
#endif
            
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

