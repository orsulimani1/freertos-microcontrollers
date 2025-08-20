"""
Auto RTC Sync for STM32 Course Project
Responds to MCU sync requests and verifies RTC time automatically
"""

import serial
import time
import datetime
import sys
import threading


def find_stm32_port():
    """Find STM32 COM port automatically"""
    import serial.tools.list_ports

    for port in serial.tools.list_ports.comports():
        if "FTDI" in port.manufacturer:
            return port.device
    return None


class RTCSync:
    def __init__(self, port, baudrate=115200):
        self.port = port
        self.baudrate = baudrate
        self.ser = None
        self.running = False

    def connect(self):
        """Connect to STM32"""
        try:
            self.ser = serial.Serial(self.port, self.baudrate, timeout=1)
            time.sleep(2)  # Wait for MCU reset
            print(f"Connected to {self.port}")
            return True
        except serial.SerialException as e:
            print(f"Connection error: {e}")
            return False

    def send_current_time(self):
        """Send current PC time to MCU"""
        now = datetime.datetime.now()
        time_cmd = f"SYNC:{now.strftime('%Y-%m-%d:%H:%M:%S')}"
        print(f"→ Sending: {time_cmd}")
        self.ser.write(time_cmd.encode() + b'\r\n')
        self.ser.flush()

    def parse_rtc_time(self, rtc_msg):
        """Parse RTC_TIME message and return datetime object"""
        try:
            # Extract time from "RTC_TIME: 2025-01-15 14:30:25"
            time_str = rtc_msg.split("RTC_TIME: ")[1].strip()
            return datetime.datetime.strptime(time_str, "%Y-%m-%d %H:%M:%S")
        except (IndexError, ValueError):
            return None

    def check_time_sync(self, rtc_time):
        """Check if RTC time matches PC time (within 4.5 seconds)"""
        pc_time = datetime.datetime.now()
        time_diff = abs((rtc_time - pc_time).total_seconds())

        print(f"PC Time:  {pc_time.strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"RTC Time: {rtc_time.strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"Difference: {time_diff:.1f} seconds")

        if time_diff <= 2:
            print("✓ Time is synchronized")
            return True
        else:
            print("✗ Time sync required - sending resync command")
            self.ser.write(b"RESYNC\r\n")
            return False

    def monitor_and_respond(self):
        """Main monitoring loop"""
        self.running = True
        print("Monitoring MCU messages...")
        print("Commands: 'sync' - manual sync, 'q' - quit")

        # Start input thread for manual commands
        input_thread = threading.Thread(target=self.handle_input, daemon=True)
        input_thread.start()

        while self.running:
            try:
                if self.ser.in_waiting > 0:
                    message = self.ser.readline().decode().strip()
                    if message:
                        if message.startswith('[') and message[9:10] == ']' and message[3:4] == ':' and message[
                                                                                                        6:7] == ':':
                            print(f"← {message}")
                        else:
                            timestamp = datetime.datetime.now().strftime('%H:%M:%S')
                            print(f"[{timestamp}] ← {message}")
                        # Handle different message types
                        if message.startswith("RTC_TIME:"):
                            rtc_time = self.parse_rtc_time(message)
                            if rtc_time:
                                if self.check_time_sync(rtc_time):
                                    # Send validation message to MCU
                                    self.ser.write(b"TIME_VALID\r\n")
                                # If sync required, RESYNC is already sent in check_time_sync

                        elif message == "SYNC_REQUEST":
                            print("MCU requesting time sync...")
                            self.send_current_time()

                        elif message.startswith("VERIFY_REQUEST"):
                            print("MCU requesting verification...")
                            # Could add verification logic here

                        elif message.startswith("SET:") or message.startswith("READ:"):
                            # Verification messages from MCU
                            pass

                        elif message.startswith("VERIFY:"):
                            if "OK" in message:
                                print("✓ MCU verification successful")
                            else:
                                print("✗ MCU verification failed")

                time.sleep(0.1)

            except KeyboardInterrupt:
                break
            except Exception as e:
                print(f"Error: {e}")
                break

        self.running = False

    def handle_input(self):
        """Handle user input commands"""
        while self.running:
            try:
                cmd = input().strip().lower()
                if cmd == 'q':
                    self.running = False
                    break
                elif cmd == 'sync':
                    self.send_current_time()
                elif cmd == 'check':
                    self.ser.write(b"CHECK\r\n")
            except:
                break

    def close(self):
        """Close connection"""
        if self.ser:
            self.ser.close()
        print("Connection closed")


def main():
    print("STM32 RTC Auto-Sync Tool")
    print("=" * 30)

    # Find STM32 port
    port = find_stm32_port()

    if not port:
        print("STM32 not found. Available ports:")
        import serial.tools.list_ports
        for p in serial.tools.list_ports.comports():
            print(f"  {p.device} - {p.description}")

        port = input("Enter COM port manually (e.g., COM3): ")

    print(f"Using port: {port}")

    # Create and start sync monitor
    rtc_sync = RTCSync(port)

    if rtc_sync.connect():
        try:
            rtc_sync.monitor_and_respond()
        except KeyboardInterrupt:
            print("\nShutting down...")
        finally:
            rtc_sync.close()

    print("Goodbye!")


if __name__ == "__main__":
    main()