/*
 * ModbusFunctions_v2.c
 *
 *  Created on: 09.09.2025.
 *      Author: dmitrof
 */

#include "GlobalData.h"

// Utility: write big-endian 16-bit
inline void put_u16_be(uint8_t* p, uint16_t v) {
    p[0] = (uint8_t)((v >> 8) & 0xFF);
    p[1] = (uint8_t)(v & 0xFF);
}

// Utility: read big-endian 16-bit
inline uint16_t get_u16_be(const uint8_t* p) {
    return (uint16_t)((p[0] << 8) | p[1]);
}


// ---- Modbus request handler on an open TCP socket ----
int handle_modbus_request(int fd) {
    uint8_t hdr[7];                         // MBAP header

    int n = recv(fd, hdr, sizeof(hdr), MSG_WAITALL);
    if (n != (int)sizeof(hdr)) return -1;

    uint16_t txid  = get_u16_be(&hdr[0]);
    uint16_t proto = get_u16_be(&hdr[2]);
    uint16_t len   = get_u16_be(&hdr[4]);   // UnitID + PDU
    uint8_t  uid   = hdr[6];

    if (proto != 0 || len < 1 || len > (1 + MODBUS_MAX_PDU)) return -1;

    uint16_t pdu_len = (uint16_t)(len - 1);
    uint8_t  pdu[MODBUS_MAX_PDU];
    n = recv(fd, pdu, pdu_len, MSG_WAITALL);
    if (n != (int)pdu_len) return -1;

    uint8_t func = pdu[0];

    // Build response
    uint8_t rsp[MODBUS_MAX_ADU];
    put_u16_be(&rsp[0], txid);
    put_u16_be(&rsp[2], 0);
    // rsp[4..5] later
    rsp[6] = uid;

    uint8_t* rpdu = &rsp[7];
    int rpdu_len = 0;

    if (func == MODBUS_FC_READ_HOLDING_REGS) {
        if (pdu_len < 5) { // Illegal Data Value
            rpdu[0]=func | 0x80;
            rpdu[1]=0x03;
            rpdu_len=2;
        }
        else {
            uint16_t start = get_u16_be(&pdu[1]);
            uint16_t qty   = get_u16_be(&pdu[3]);
            if (qty == 0 || qty > 125) { // per spec, max 125 regs
                rpdu[0]=func | 0x80;
                rpdu[1]=0x03;
                rpdu_len=2;
            }
            else if ((uint32_t)start + qty > REG_HOLDING_NREGS) { // Illegal Address
                rpdu[0]=func | 0x80;
                rpdu[1]=0x02;
                rpdu_len=2;
            }
            else {
                rpdu[0] = func;
                rpdu[1] = (uint8_t)(qty * 2);

                uint16_t i = 0;
                for (; i < qty; ++i) {
//                    put_u16_be(&rpdu[2 + 2*i], usRegHoldingBuf[start + i]);
                    put_u16_be(&rpdu[2 + 2*i], CTOM_Data[start + i - MTOC_DATA_SIZE]);
                }

                rpdu_len = 2 + qty * 2;
            }
        }
    }
    else if (func == MODBUS_FC_WRITE_SINGLE_REG) {

        // Request PDU: 06 addrHi addrLo valHi valLo  (length must be 5)
        if (pdu_len != 5) { // Illegal Data Value
            rpdu[0] = func | 0x80;
            rpdu[1] = 0x03;
            rpdu_len = 2;
        } else {
            uint16_t addr = get_u16_be(&pdu[1]);
            uint16_t val  = get_u16_be(&pdu[3]);

            if (addr >= REG_HOLDING_NREGS) {  // Illegal Data Address
                rpdu[0] = func | 0x80;
                rpdu[1] = 0x02;
                rpdu_len = 2;
            } else {
                // Write is protected by a mutex (binary semaphore)
                if (g_regLock) Semaphore_pend(g_regLock, BIOS_WAIT_FOREVER);



                if ((addr == 5) || (ModbusControlledInput == 1))
                {
                    if (addr < MTOC_DATA_SIZE) {
                        MTOC_Data[addr] = val;
                    }
                }

                if (MTOC_Data[5]==2015)  {ModbusControlledInput = 0;}
                                            else {ModbusControlledInput = 1;}

                //g_holdingRegs[addr] = val;
                if (g_regLock) Semaphore_post(g_regLock);

                // Response echoes address and value (per Modbus spec)
                rpdu[0] = func;
                put_u16_be(&rpdu[1], addr);
                put_u16_be(&rpdu[3], val);
                rpdu_len = 5;
            }
        }
    }
    else { // Illegal Function
        rpdu[0] = func | 0x80;
        rpdu[1] = 0x01;
        rpdu_len = 2;
    }

    put_u16_be(&rsp[4], (uint16_t)(1 + rpdu_len)); // UnitID + PDU
    int adu_len = 7 + rpdu_len;

    n = send(fd, rsp, adu_len, 0);
    if (n != adu_len) return -1;

    return 0;
}


// ---- Worker Task: one per client connection ----
Void ModbusWorkerTask(UArg arg0, UArg arg1) {
    int fd = (int)(uintptr_t)arg0;
    (void)arg1;

    // Each Task that uses BSD sockets must open/close a session
    fdOpenSession(TaskSelf());

    // Optional: set socket timeouts to avoid stuck connections
    //struct timeval tv; tv.tv_sec = 30; tv.tv_usec = 0;
    //setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    //setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    // Serve requests until peer closes or error occurs
    while (1) {
        int rc = handle_modbus_request(fd);
        if (rc < 0) break;
    }

    close(fd);
    fdCloseSession(TaskSelf());

    // Release a connection slot
    if (g_connSlots) Semaphore_post(g_connSlots);

    Task_exit();
}

Void ModbusServerTask(UArg arg0, UArg arg1) {
    (void)arg0; (void)arg1;

    // Required per-task when using NDK BSD sockets
    fdOpenSession(TaskSelf());

    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_fd < 0) {
        fdCloseSession(TaskSelf());
        Task_exit();
        return;
    }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(MODBUS_TCP_PORT);

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(listen_fd);
        fdCloseSession(TaskSelf());
        Task_exit();
        return;
    }

    if (listen(listen_fd, 2) < 0) {
        close(listen_fd);
        fdCloseSession(TaskSelf());
        Task_exit();
        return;
    }

    for (;;) {
        // Block until a slot is available (back-pressure on accept)
        Semaphore_pend(g_connSlots, BIOS_WAIT_FOREVER);

        struct sockaddr_in cli;
        socklen_t clen = sizeof(cli);
        int client_fd = accept(listen_fd, (struct sockaddr*)&cli, &clen);
        if (client_fd < 0) {
            // accept failed; free slot and continue
            Semaphore_post(g_connSlots);

            // transient errors possible; continue
            continue;
        }

        // Spawn a worker Task for this client
        Task_Params tp;
        Task_Params_init(&tp);
        tp.instance->name = (xdc_String)"ModbusWorker";
        tp.priority = 3;
        tp.stackSize = 2048;                // bump if you add more functions
        tp.arg0 = (UArg)(uintptr_t)client_fd;
        if (Task_create(ModbusWorkerTask, &tp, NULL) == NULL) {
            // If we fail to create a worker, close client and release slot
            close(client_fd);
            Semaphore_post(g_connSlots);
        }

    }

    // (normally unreachable)
    close(listen_fd);
    fdCloseSession(TaskSelf());
    Task_exit();
}

