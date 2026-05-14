#!/usr/bin/env python3
import os
import serial
import time

IMAGE = "cartridge.bin"
PORT = "/dev/ttyACM0"
BAUD = 2000000

OUTPUT = "eeprom_dump.bin"
CHIP_SIZE = 4 * 1024 * 1024
ser = None


def dump():
    print("\n=== Starting Dump ===\n")

    received = 0
    with open(OUTPUT, "wb") as f:
        while received < CHIP_SIZE:
            remaining = CHIP_SIZE - received
            chunk = ser.read(min(4096, remaining))

            if not chunk:
                print("\nTimeout!")
                break

            f.write(chunk)
            received += len(chunk)

    print()

    while ser.in_waiting:
        line = ser.readline().decode(errors="ignore").strip()
        if line:
            print(f"Pico: {line}")

    print(f"Saved to: {OUTPUT}")
    print(f"Size: {os.path.getsize(OUTPUT)} bytes")


def send():
    with open(IMAGE, "rb") as f:
        data = f.read()

    print(f"Loaded {len(data):,} bytes")
    print("\n=== Starting Send ===\n")

    chunk_size = 65536
    sent = 0
    try:
        while sent < len(data):
            while sent < len(data):
                chunk = data[sent:sent + chunk_size]
                ser.write(chunk)
                sent += len(chunk)

        print("\n\nSending complete!")

    except KeyboardInterrupt:
        print("\n\nSending interrupted.")
    except Exception as e:
        print(f"\nError: {e}")


def compare_files(file1_path, file2_path):
    print("Verifying dump")
    with open(file1_path, "rb") as f1, open(file2_path, "rb") as f2:

        offset = 0
        chunk_size = 4096

        while True:
            b1 = f1.read(chunk_size)
            b2 = f2.read(chunk_size)

            # End of both files
            if not b1 and not b2:
                print("Files are identical")
                return True

            # Different lengths
            if len(b1) != len(b2):
                print("File sizes differ")
                print(f"Offset: 0x{offset:08X}")
                print(f"{file1_path}: {len(b1)} bytes remaining")
                print(f"{file2_path}: {len(b2)} bytes remaining")
                return False

            # Compare bytes
            for i in range(len(b1)):
                if b1[i] != b2[i]:
                    fail_offset = offset + i

                    print("Files differ")
                    print(f"Offset : 0x{fail_offset:08X}")
                    print(f"{file1_path}: 0x{b1[i]:02X}")
                    print(f"{file2_path}: 0x{b2[i]:02X}")

                    # Context dump
                    start = max(0, i - 16)
                    end = min(len(b1), i + 16)

                    print("\nContext:")
                    print(f"{file1_path}:")
                    print(" ".join(f"{x:02X}" for x in b1[start:end]))

                    print(f"\n{file2_path}:")
                    print(" ".join(f"{x:02X}" for x in b2[start:end]))

                    return False

            offset += len(b1)


while True:
    try:
        print("Attempting reconnect...")
        ser = serial.Serial(PORT, BAUD, timeout=0.5)  # Short timeout
        print("Waiting for Pico...")
        while True:
            line = ser.readline().decode(errors='ignore').strip()
            if line:
                print(f"Pico: {line}")
            if "BEGIN_FLASH" in line:
                send()
                print("Flashing Complete")
            if "BEGIN_VERIFY" in line:
                send()
                print("Verification Complete")
            if "BEGIN_DUMP" in line:
                dump()
                print("Dumping Complete")
                compare_files("cartridge.bin", "eeprom_dump.bin")

    except:
        print(f"Connection lost")
        if ser:
            try:
                ser.close()
            except:
                pass
        ser = None
        time.sleep(1)
