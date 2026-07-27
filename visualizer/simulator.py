import socket
import struct
import threading
import random
import time

# --- Configuration ---
HOST = "0.0.0.0"  # Listen on all available interfaces (localhost / LAN)
PORT = 1124       # Default port used by your C++ monitor application

# --- Protocol Constants ---
# Sizes must match #pragma pack(1) structs in C++
SIZE_OF_OSCI_ERRORS = 8
SIZE_OF_OSCI_PACKET = 32
SIZE_OF_OSCI_REQUEST = 4
SIZE_OF_OSCI_RESPONSE = 12
SIZE_OF_INFO_DATA = 4

PACKET_CMD_ECHO = 0
PACKET_CMD_OSCI = 1
PACKET_CMD_FULL = 2
PACKET_CMD_INFO = 3

def recvall(sock, n):
    """Helper to receive exactly n bytes. TCP streams can fragment."""
    data = bytearray()
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None  # Connection closed by client
        data.extend(packet)
    return data

def generate_osci_packet(counter):
    """Generate a dummy 32-byte Osci_Packet with random sensor values."""
    # CycleCounter[4] (4 x Uint16) = 8 bytes
    cc = struct.pack("<HHHH", counter, counter, counter, counter)
    
    # Osci_Errors (4 x Uint16) = 8 bytes
    err = struct.pack("<HHHH", 1, 2, 3, 4)
    
    # Sensor data (6 x Uint16) = 12 bytes
    vals = struct.pack("<HHHHHH", 
                       random.randint(0, 1000),  # Current_1
                       random.randint(0, 1000),  # Current_2
                       random.randint(0, 5000),  # Voltage_Inp
                       random.randint(0, 5000),  # Voltage_Out
                       random.randint(0, 1000),  # FreeTimeCounter
                       random.randint(0, 100))   # WatchDog
    
    # __pad[2] (2 x Uint16) = 4 bytes
    pad = struct.pack("<HH", 0, 0)
    
    return cc + err + vals + pad

def handle_client(conn, addr):
    """Handle a single TCP client connection."""
    print(f"\n[NEW CONNECTION] {addr} connected.")
    connected = True
    
    try:
        while connected:
            # 1. Receive the 4-byte Osci_Request
            req_data = recvall(conn, SIZE_OF_OSCI_REQUEST)
            if not req_data:
                print(f"[DISCONNECT] {addr} closed the connection.")
                break
            
            # <HH means little-endian unsigned short (2 bytes) x 2
            cmd, arg = struct.unpack("<HH", req_data)
            print(f"[RECV] {addr} -> cmd={cmd}, arg={arg}")

            if cmd == PACKET_CMD_ECHO:
                # Respond with Osci_Response (cmd, len=0, errors)
                # <HH8s = cmd(2) + len(2) + errors(8) = 12 bytes
                resp = struct.pack("<HHHHHH", PACKET_CMD_ECHO, 0, 1, 2, 3, 4)
                conn.sendall(resp)
                print(f"[SEND] {addr} <- ECHO Response")

            elif cmd == PACKET_CMD_OSCI:
                num_packets = arg
                payload_len = num_packets * SIZE_OF_OSCI_PACKET
                
                # Send Response Header
                resp = struct.pack("<HHHHHH", PACKET_CMD_OSCI, payload_len, 6, 7, 6, 7)
                conn.sendall(resp)
                print(f"[SEND] {addr} <- OSCI Response (len={payload_len})")
                
                # Send Payload
                payload = bytearray()
                for i in range(num_packets):
                    payload.extend(generate_osci_packet(i))
                conn.sendall(payload)
                print(f"[SEND] {addr} <- {num_packets} OSCI Packets")

            elif cmd == PACKET_CMD_FULL:
                # Send some dummy FULL packets (e.g., 10 packets)
                num_packets = 10
                payload_len = num_packets * SIZE_OF_OSCI_PACKET
                
                # Send FULL Response Header
                resp = struct.pack("<HHHHHH", PACKET_CMD_FULL, payload_len, 123, 456, 789, 69)
                conn.sendall(resp)
                print(f"[SEND] {addr} <- FULL Response (len={payload_len})")
                
                # Send FULL Payload
                payload = bytearray()
                for i in range(num_packets):
                    payload.extend(generate_osci_packet(i))
                conn.sendall(payload)
                print(f"[SEND] {addr} <- {num_packets} FULL Packets")

                # 2. Wait for the follow-up PACKET_CMD_INFO request
                req_data = recvall(conn, SIZE_OF_OSCI_REQUEST)
                if not req_data:
                    break
                cmd2, arg2 = struct.unpack("<HH", req_data)
                print(f"[RECV] {addr} -> cmd={cmd2}, arg={arg2}")

                if cmd2 != PACKET_CMD_INFO:
                    print(f"[ERROR] Expected PACKET_CMD_INFO, got {cmd2}. Disconnecting.")
                    break

                # Send INFO Response Header
                info_resp = struct.pack("<HH8s", PACKET_CMD_INFO, SIZE_OF_INFO_DATA, b'\x00'*8)
                conn.sendall(info_resp)
                print(f"[SEND] {addr} <- INFO Response")

                # Send INFO Payload (before_offset, after_offset)
                info_payload = struct.pack("<HH", 12345, 6767)
                conn.sendall(info_payload)
                print(f"[SEND] {addr} <- INFO Payload")
                
                # Note: If g_repeat is checked in C++, the client will loop 
                # and send PACKET_CMD_FULL again on this exact same socket connection.

            else:
                print(f"[ERROR] Unknown command {cmd}. Disconnecting.")
                break

    except ConnectionResetError:
        print(f"[DISCONNECT] {addr} connection reset by peer.")
    finally:
        conn.close()
        print(f"[CLOSED] Connection with {addr} terminated.")

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Allow port reuse to quickly restart the script
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((HOST, PORT))
    server.listen()
    
    print(f"=====================================================")
    print(f" MCU Simulator is listening on {HOST}:{PORT}")
    print(f" Start your C++ monitor app and test the buttons!")
    print(f"=====================================================")
    
    try:
        while True:
            conn, addr = server.accept()
            thread = threading.Thread(target=handle_client, args=(conn, addr))
            thread.start()
            print(f"[ACTIVE CONNECTIONS] {threading.active_count() - 1}")
    except KeyboardInterrupt:
        print("\n[SHUTTING DOWN] Server stopped by user.")
    finally:
        server.close()

if __name__ == "__main__":
    start_server()
