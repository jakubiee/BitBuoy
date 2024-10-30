import serial
import serial.tools.list_ports
import time

def list_usb_ports():
    """List all available USB ports."""
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]

def select_usb_port():
    """Allow the user to select a USB port with option to refresh."""
    while True:
        ports = list_usb_ports()
        
        if not ports:
            print("No USB ports found.")
        else:
            print("\nAvailable USB ports:")
            for idx, port in enumerate(ports):
                print(f"{idx + 1}: {port}")
        
        print("\nOptions:")
        print("r: Refresh list")
        print("q: Quit")
        
        user_input = input("Select a USB port by number or choose an option: ").strip().lower()
        
        if user_input == 'q':
            print("Exiting...")
            return None
        elif user_input == 'r':
            print("Refreshing list...")
            time.sleep(1)
            continue
        else:
            try:
                choice = int(user_input) - 1
                if 0 <= choice < len(ports):
                    return ports[choice]
                else:
                    print("Invalid selection. Try again.")
            except ValueError:
                print("Invalid input. Enter a number, 'r', or 'q'.")

def serial_monitor(port_name):
    """Connect to the selected USB port and print received data."""
    try:
        with serial.Serial(port_name, 9600, timeout=1) as ser:
            print(f"Connected to {port_name}. Press Ctrl+C to exit.")
            while True:
                if ser.in_waiting > 0:
                    data = ser.readline().decode('utf-8').strip()
                    print(data)
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    selected_port = select_usb_port()
    if selected_port:
        serial_monitor(selected_port)
