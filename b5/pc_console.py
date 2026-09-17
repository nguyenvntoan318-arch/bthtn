#!/usr/bin/env python3
"""Cong cu PC gui lenh UART. Khong phai firmware nap vao STM32."""
import argparse
import sys
import time
try:
    import serial
    from serial.tools import list_ports
except ImportError:
    sys.exit("Thieu pyserial. Chay: sudo apt install python3-serial")

def main():
    parser = argparse.ArgumentParser(description="Bai05: UART 9600 8N1")
    parser.add_argument("--port", help="Vi du /dev/ttyUSB0")
    args = parser.parse_args()
    port = args.port
    if not port:
        ports = sorted(list_ports.comports(), key=lambda p: p.device)
        if not ports:
            sys.exit("Khong thay cong. Gan USB-UART cho Ubuntu: VirtualBox > Devices > USB.")
        for i, p in enumerate(ports, 1):
            print(f"{i}. {p.device} - {p.description}")
        if len(ports) == 1:
            port = ports[0].device
        else:
            try:
                choice = int(input("Chon so cua USB-UART: "))
                if choice < 1 or choice > len(ports):
                    raise ValueError
                port = ports[choice - 1].device
            except ValueError:
                sys.exit("So khong hop le. Chay lai chuong trinh.")
    with serial.Serial(port, 9600, timeout=0.1, write_timeout=2,
                       bytesize=8, parity="N", stopbits=1,
                       xonxoff=False, rtscts=False, dsrdtr=False) as link:
        print(f"Da mo {port}: 9600, 8N1. Nhap lenh co ! roi Enter. Ctrl+C de thoat.")
        print("Vi du: ON!   OFF!   PWM:30%!   Status!")
        link.reset_input_buffer()
        while True:
            command = input("PC> ").strip()
            if not command:
                continue
            if not command.endswith("!") or command.count("!") != 1:
                print("Nhap mot lenh moi lan va ket thuc bang !, vi du PWM:30%!")
                continue
            try:
                data = command.encode("ascii")
            except UnicodeEncodeError:
                print("Lenh chi dung ky tu khong dau.")
                continue
            link.write(data)
            link.flush()
            received = bytearray()
            deadline = time.monotonic() + 2
            while time.monotonic() < deadline:
                received.extend(link.read(128))
                if b"\n" in received:
                    break
            if received:
                print("STM32>", received.decode("ascii", errors="replace").rstrip())
            else:
                print("Chua co phan hoi: kiem tra nguon, TX/RX/GND, cong va firmware.")

if __name__ == "__main__":
    try:
        main()
    except (KeyboardInterrupt, EOFError):
        print("\nDa dong cong UART.")
    except serial.SerialException as exc:
        print(f"Loi cong UART: {exc}", file=sys.stderr)
        print('Neu Permission denied: sudo usermod -aG dialout "$USER", roi dang xuat/dang nhap Ubuntu.', file=sys.stderr)
        sys.exit(1)
