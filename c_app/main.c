#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
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

#define IP "10.1.3.12"
#define PORT 1124

void print_errors(Osci_Errors errors) {
    printf(
        "Uint16 C28_Errors = %u\n"
        "Uint16 C28_Errors_Latch = %u\n"
        "Uint16 FPGA_Errors = %u\n"
        "Uint16 FPGA_Errors_Latch = %u\n",
        errors.C28_Errors,
        errors.C28_Errors_Latch,
        errors.FPGA_Errors,
        errors.FPGA_Errors_Latch
    );
}

void print_responce(Osci_Response resp) {
    printf(
        "Received:\n"
        "Uint16 cmd = %u\n"
        "Uint16 len = %u\n"
        "Osci_Errors errors = {\n",
        resp.cmd, resp.len
    );
    print_errors(resp.errors);
    printf("}\n");
}

void print_packet(Osci_Packet packet) {
    printf(
        "Uint16 CycleCounter[4] = %zu\n"
        "Uint16 Current_1 = %u\n"
        "Uint16 Current_2 = %u\n"
        "Uint16 Voltage_Inp = %u\n"
        "Uint16 Voltage_Out = %u\n"
        "Uint16 FreeTimeCounter = %u\n"
        "Uint16 WatchDog = %u\n"
//        "Uint16 __pad[2] = %u\n"
        "Osci_Errors errors = {\n",
        *((uint64_t*)(&packet.CycleCounter)),
        packet.Current_1,
        packet.Current_2,
        packet.Voltage_Inp,
        packet.Voltage_Out,
        packet.FreeTimeCounter,
        packet.WatchDog
    );
    print_errors(packet.errors);
    printf("}\n");
}

void send_echo(int sock) {
    int status = 0;
    Osci_Request data = { .cmd = PACKET_CMD_ECHO, .arg = 123 };
    status = tcp_send_all(sock, &data, sizeof(data));
    if (status < 0) { perror("send"); exit(-1); }
    printf("Sent ECHO\n");

    Osci_Response resp = {0};
    status = tcp_recv_all(sock, &resp, sizeof(resp));
    if (status < 0) { perror("recv header"); exit(-1); }
    assert(resp.cmd == PACKET_CMD_ECHO && "Expected ECHO response");

    print_responce(resp);
}

void send_osci(int sock, Uint16 num_packets) {
    int status = 0;
    Osci_Request data = { .cmd = PACKET_CMD_OSCI, .arg = num_packets };
    status = tcp_send_all(sock, &data, sizeof(data));
    if (status < 0) { perror("send"); exit(-1); }
    printf("Sent OSCI\n");

    Osci_Response resp = {0};
    status = tcp_recv_all(sock, &resp, sizeof(resp));
    if (status < 0) { perror("recv header"); exit(-1); }
    assert(resp.cmd == PACKET_CMD_OSCI && "Expected OSCI response");
    print_responce(resp);
    printf("Received OSCI response header\n");

    Osci_Packet *packets = calloc(resp.len, 1);
    status = tcp_recv_all(sock, packets, resp.len);
    if (status < 0) { perror("recv body"); exit(-1); }
    double got_packets = resp.len / sizeof(Osci_Packet);
    printf(
        "Received OSCI packets: %u (bytes); requested %u packets, got %f\n",
        resp.len, num_packets, got_packets
    );
    for (uint32_t i = 0; i < (uint32_t)got_packets; i++) {
        printf("\tPacket %u\n", i);
        print_packet(packets[i]);
        printf("\n");
    }
}

void send_full(int sock) {
    int status = 0;
    Osci_Request data = { .cmd = PACKET_CMD_FULL, .arg = 123 };
    status = tcp_send_all(sock, &data, sizeof(data));
    if (status < 0) { perror("send"); exit(-1); }
    printf("Sent FULL\n");

    Osci_Response resp = {0};
    status = tcp_recv_all(sock, &resp, sizeof(resp));
    if (status < 0) { perror("recv header"); exit(-1); }
    assert(resp.cmd == PACKET_CMD_FULL && "Expected FULL response");
    print_responce(resp);
    printf("Received FULL response header\n");

    Osci_Packet *packets = calloc(resp.len, 1);
    status = tcp_recv_all(sock, packets, resp.len);
    double got_packets = resp.len / sizeof(Osci_Packet);
    printf(
        "Received FULL packets: %u (bytes); %f (packets)\n",
        resp.len, got_packets
    );
    for (uint32_t i = 0; i < (uint32_t)got_packets; i++) {
        printf("\tPacket %u\n", i);
        print_packet(packets[i]);
        printf("\n");
    }

    status = tcp_recv_all(sock, &resp, sizeof(resp));
    if (status < 0) { perror("recv offset"); exit(-1); }
    printf("Received FULL INFO response header\n");
    assert(resp.cmd == PACKET_CMD_INFO && "Expected INFO response");

#pragma pack(1)
    struct {
        Uint16 before_offset;
        Uint16 after_offset;
    } info_data = {0};
#pragma pack()
    assert(
        resp.len == sizeof(info_data) &&
        "Mismatched INFO header response length"
    );
    tcp_recv_all(sock, &info_data, sizeof(info_data));

    printf(
        "Received FULL INFO response body: "
        "before_offset = %u; after_offset = %u\n",
        info_data.before_offset, info_data.after_offset
    );
}

int main(int argc, char **argv) {
    int status = 0;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_aton(IP, &server_address.sin_addr);

    status = connect(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (status < 0) { perror("Connect"); return -1; }

    printf("CONNECTED\n\n");
    char cmd = 'e';
    if (argc >= 2) {
        cmd = argv[1][0];
    }
    switch (cmd) {
        case 'o': {
            Uint16 num_packets = 16;
            if (argc >= 3) {
                num_packets = atoi(argv[2]);
            }
            send_osci(sock, num_packets);
        } break;

        case 'f': {
            send_full(sock);
        } break;

        case 'e': {
            send_echo(sock);
        } break;
    }

    close(sock);
    return 0;
}

