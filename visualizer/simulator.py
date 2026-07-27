import socket
import struct
import threading
import math
import time

# --- Configuration ---
HOST = "0.0.0.0"
PORT = 1124

# --- Protocol Constants ---
SIZE_OF_OSCI_PACKET = 32
SIZE_OF_OSCI_REQUEST = 4
SIZE_OF_INFO_DATA = 4

PACKET_CMD_ECHO = 0
PACKET_CMD_OSCI = 1
PACKET_CMD_FULL = 2
PACKET_CMD_INFO = 3

# --- Simulation State ---
# Global 64-bit tick counter to simulate advancing time
global_time_ticks = 0
TICKS_PER_PACKET = 1000  # Advance time by 1000 units per packet

def recvall(sock, n):
    data = bytearray()
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet: return None
        data.extend(packet)
    return data

def generate_osci_packet(packet_index):
    """Generate a 32-byte Osci_Packet with sine wave sensors and advancing time."""
    global global_time_ticks
    
    # 1. ADVANCING TIME (64-bit CycleCounter split into 4 x uint16_t)
    t64 = global_time_ticks
    global_time_ticks += TICKS_PER_PACKET
    
    # Split 64-bit int into 4 little-endian 16-bit chunks
    cc0 = t64 & 0xFFFF
    cc1 = (t64 >> 16) & 0xFFFF
    cc2 = (t64 >> 32) & 0xFFFF
    cc3 = (t64 >> 48) & 0xFFFF
    cc = struct.pack("<HHHH", cc0, cc1, cc2, cc3)
    
    # 2. SINE WAVES FOR SENSORS
    # Use packet_index as the time variable for the sine waves
    t = packet_index * 0.05 
    
    # Current_1: Fast frequency, medium amplitude
    val1 = int(1000 * math.sin(t * 2.0) + 2000)
    # Current_2: Slow frequency, large amplitude
    val2 = int(2000 * math.sin(t * 0.5) + 3000)
    # Voltage_Inp: Very slow drift, high baseline
    val3 = int(5000 * math.sin(t * 0.1) + 25000)
    # Voltage_Out: Slightly different freq/phase to show relationship
    val4 = int(4500 * math.sin(t * 0.12 + 1.0) + 24000)
    
    # Clamp values to uint16 bounds (0 - 65535)
    val1 = max(0, min(65535, val1))
    val2 = max(0, min(65535, val2))
    val3 = max(0, min(65535, val3))
    val4 = max(0, min(65535, val4))
    
    sensors = struct.pack("<HHHH", val1, val2, val3, val4)
    
    # 3. MISC COUNTERS
    free_time = int(500 * math.sin(t * 0.8) + 500)
    watchdog = (packet_index % 256) # Simple rolling 8-bit counter in 16-bit space
    misc = struct.pack("<HH", free_time, watchdog)
    
    # 4. ERRORS (Set to 0 for clean data. Remember: errors are sent in the HEADER!)
    err = struct.pack("<HHHH", 0, 0, 0, 0)
    
    # 5. PADDING
    pad = struct.pack("<HH", 0, 0)
    
    # Total: 8 (cc) + 8 (err) + 8 (sensors) + 4 (misc) + 4 (pad) = 32 bytes
    return cc + err + sensors + misc + pad

def handle_client(conn, addr):
    print(f"\n[NEW CONNECTION] {addr} connected.")
    
    try:
        while True:
            req_data = recvall(conn, SIZE_OF_OSCI_REQUEST)
            if not req_data:
                print(f"[DISCONNECT] {addr} closed.")
                break
            
            cmd, arg = struct.unpack("<HH", req_data)
            print(f"[RECV] {addr} -> cmd={cmd}, arg={arg}")

            if cmd == PACKET_CMD_ECHO:
                # Header: cmd, len=0, errors=1,2,3,4 (to test your error UI)
                resp = struct.pack("<HHHHHH", PACKET_CMD_ECHO, 0, 1, 2, 3, 4)
                conn.sendall(resp)

            elif cmd == PACKET_CMD_OSCI:
                num_packets = arg
                payload_len = num_packets * SIZE_OF_OSCI_PACKET
                
                # Header with errors 1,2,3,4
                resp = struct.pack("<HHHHHH", PACKET_CMD_OSCI, payload_len, 1, 2, 3, 4)
                conn.sendall(resp)
                
                # Payload with sine waves
                payload = bytearray()
                for i in range(num_packets):
                    payload.extend(generate_osci_packet(i))
                conn.sendall(payload)

            elif cmd == PACKET_CMD_FULL:
                num_packets = 10
                payload_len = num_packets * SIZE_OF_OSCI_PACKET
                
                # Header with errors 1,2,3,4
                resp = struct.pack("<HHHHHH", PACKET_CMD_FULL, payload_len, 1, 2, 3, 4)
                conn.sendall(resp)
                
                payload = bytearray()
                for i in range(num_packets):
                    payload.extend(generate_osci_packet(i))
                conn.sendall(payload)

                # Wait for INFO request
                req_data = recvall(conn, SIZE_OF_OSCI_REQUEST)
                if not req_data: break
                cmd2, arg2 = struct.unpack("<HH", req_data)

                if cmd2 == PACKET_CMD_INFO:
                    info_resp = struct.pack("<HHHHHH", PACKET_CMD_INFO, SIZE_OF_INFO_DATA, 0, 0, 0, 0)
                    conn.sendall(info_resp)
                    info_payload = struct.pack("<HH", 12345, 6790)
                    conn.sendall(info_payload)
                else:
                    break

            else:
                break

    except ConnectionResetError:
        pass
    finally:
        conn.close()
        print(f"[CLOSED] {addr}")

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((HOST, PORT))
    server.listen()
    
    print(f"=====================================================")
    print(f" MCU Simulator (Sine Waves) listening on {HOST}:{PORT}")
    print(f"=====================================================")
    
    try:
        while True:
            conn, addr = server.accept()
            thread = threading.Thread(target=handle_client, args=(conn, addr))
            thread.start()
    except KeyboardInterrupt:
        print("\n[SHUTTING DOWN]")
    finally:
        server.close()

if __name__ == "__main__":
    start_server()
