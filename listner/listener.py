import serial
import serial.tools.list_ports
import yaml
import time

# Configuration
BAUD_RATE = 115200
YAML_FILE = 'points.yaml'

def find_pendant_port():
    """Scans all USB ports and finds the Pico automatically."""
    ports = serial.tools.list_ports.comports()
    
    # Pass 1: Look for Raspberry Pi Pico by its official USB Vendor ID (0x2E8A)
    for port in ports:
        if port.vid == 0x2E8A:
            return port.device
            
    # Pass 2: Fallback for generic Arduino/Pico connections on Ubuntu and Mac
    for port in ports:
        if 'ttyACM' in port.device or 'cu.usbmodem' in port.device:
            return port.device
            
    return None

def load_data():
    """Reads the full YAML file structure."""
    try:
        with open(YAML_FILE, 'r') as f:
            data = yaml.safe_load(f)
            if not data:
                return {'points_file_name': 'points', 'points': []}
            if 'points' not in data:
                data['points'] = []
            return data
    except Exception as e:
        print(f"Error reading YAML file: {e}")
        return {'points_file_name': 'points', 'points': []}

def send_command(ser, command):
    """Formats and sends a serial command ending with a newline."""
    cmd_str = f"{command}\n"
    ser.write(cmd_str.encode('utf-8'))
    ser.flush()
    time.sleep(0.05)

def send_points_list(ser, points_list):
    """Sends all point names to the Pico and prompts it to show the list."""
    send_command(ser, "POINTS|CLEAR")
    for pt in points_list:
        name = pt.get('name', 'Unknown')
        send_command(ser, f"POINTS|ADD|{name}")
    send_command(ser, "POINTS|SHOW")

def start_listener():
    """Main loop handling auto-detection and reconnects."""
    
    print("Starting Pendant Serial Listener for Mac...")

    while True:
        try:
            # 1. AUTO-DETECT THE PORT
            port_name = find_pendant_port()
            
            if not port_name:
                print("Waiting for Pendant to be plugged in...")
                time.sleep(2)
                continue # Loop back and search again

            print(f"Pendant detected on {port_name}! Attempting to connect...")
            
            # 2. CONNECT TO THE DETECTED PORT
            with serial.Serial(port_name, BAUD_RATE, timeout=1) as ser:
                print("Successfully connected! Listening for commands...")
                time.sleep(1) 
                
                while True:
                    if ser.in_waiting > 0:
                        line = ser.readline().decode('utf-8', errors='ignore').strip()
                        if not line:
                            continue

                        full_data = load_data()
                        points_list = full_data.get('points', [])

                        if line in ("PENDANT|READY", "PENDANT|POINTS|REQUEST"):
                            send_command(ser, "POINTS|READY")

                        elif line == "PENDANT|POINTS|GET_LIST":
                            send_points_list(ser, points_list)

                        # --- UPDATE LOGIC (ROS Removed) ---
                        elif line.startswith("PENDANT|POINT|UPDATE|"):
                            try:
                                idx_str = line.split("|")[-1]
                                idx = int(idx_str)
                                
                                if 0 <= idx < len(points_list):
                                    pt_name = points_list[idx].get('name', 'Unknown')
                                    # Just print to the console instead of publishing to ROS
                                    print(f"[ACTION] -> {pt_name} is updated")
                                else:
                                    print(f"Update index {idx} out of range.")
                            except ValueError:
                                print("Failed to parse update index.")

        except serial.SerialException as e:
            print(f"\n[Warning] Serial connection lost. Did you unplug the Pendant?")
            time.sleep(2)
        except OSError as e:
            print(f"\n[Warning] OS Error: Device disconnected.")
            time.sleep(2)
        except KeyboardInterrupt:
            print("\nExiting script.")
            break
        except Exception as e:
            print(f"\n[Error] Unexpected error: {e}")
            time.sleep(2)

if __name__ == "__main__":
    start_listener()