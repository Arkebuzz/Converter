#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int tcp_send_all(int fd, volatile const void *buf, size_t len) {
    if (len == 0) {
        return 0;
    }
    size_t bytes_sent = 0;
    while (bytes_sent < len) {
        int sent = send(
            fd,
            ((uint8_t *)buf) + bytes_sent,
            len - bytes_sent,
            0
        );
        if (sent <= 0) {
            printf("tcp_send_all: send() failed\n");
            return -1;
        }
        bytes_sent += sent;
    }
    return 0;
}

int tcp_recv_all(int fd, volatile const void *buf, size_t len) {
    size_t bytes_read = 0;
    while (bytes_read < len) {
        int read = recv(
            fd,
            ((uint8_t *)buf) + bytes_read,
            len - bytes_read,
            0
        );
        if (read <= 0) {
            printf("tcp_recv_all: read() failed\n");
            return -1;
        }
        bytes_read += read;
    }
    return 0;
}

typedef uint16_t Uint16;

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

#define IP "10.1.3.12"
#define PORT 1124
#define PACKET_CMD_ECHO 0
#define PACKET_CMD_OSCI 1
#define PACKET_CMD_FULL 2

void print_errors(Osci_Errors errors) {
    printf(
        "Uint16 C28_Errors = %i\n"
        "Uint16 C28_Errors_Latch = %i\n"
        "Uint16 FPGA_Errors = %i\n"
        "Uint16 FPGA_Errors_Latch = %i\n",
        errors.C28_Errors,
        errors.C28_Errors_Latch,
        errors.FPGA_Errors,
        errors.FPGA_Errors_Latch
    );
}

void print_responce(Osci_Response resp) {
    printf("Received:\n"
    "Uint16 cmd = %i\n"
    "Uint16 len = %i\n"
    "Osci_Errors errors = {\n",
    resp.cmd, resp.len
    );
    print_errors(resp.errors);
    printf("}\n");
}

void send_echo(int sock) {
    int status = 0;
    Osci_Request data = { .cmd = PACKET_CMD_ECHO, .arg = 123};
    status = tcp_send_all(sock, &data, sizeof(data));
    if (status < 0) { perror("recv"); exit(-1); }
    printf("Sent ECHO\n");

    Osci_Response resp = {0};
    status = tcp_recv_all(sock, &resp, sizeof(resp));
    if (status < 0) { perror("recv"); exit(-1); }

    print_responce(resp);
}

int main(void) {
    int status = 0;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT); // Port number
    inet_aton(IP, &server_address.sin_addr); // Server IP

    // Connect to the server
    status = connect(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (status < 0) { perror("Connect"); return -1; }

    printf("Connected\n");
    send_echo(sock);

    // Close the socket
    close(sock);
    return 0;
}

