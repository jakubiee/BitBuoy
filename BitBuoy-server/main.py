import serial
import serial.tools.list_ports
import time
import requests
import json
from datetime import datetime,timezone

def list_usb_ports():
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]

def select_usb_port():
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
    try:
        with serial.Serial(port_name, 9600, timeout=1) as ser:
            print(f"Connected to {port_name}. Press Ctrl+C to exit.")
            while True:
                if ser.in_waiting > 0:
                    data = ser.readline().decode('utf-8').strip()
                    data = data[10:]
                    if data.startswith("ABCD1234"):
                        send_measurement(data.split(","))

    except Exception as e:
        print(f"Error: {e}")



def send_measurement(data):
    url = 'https://bitbuoy-backend-production.up.railway.app/measurement/'
    headers = {
        'accept': 'application/json',
        'token': 'securetoken123',
        'Content-Type': 'application/json'
    }
    
    data = {
        "buoy_serial_number": data[0],
        "ambient_temp": float(data[1]),
        "water_temp": float(data[2]),
        "water_pollution": float(data[3]),
        "humidity": float(data[4]),
        "lat": float(data[5]),
        "long": float(data[6]),
        "timestamp": datetime.now(timezone.utc).isoformat() 
    }
    
    try:
        response = requests.post(url, headers=headers, data=json.dumps(data))
        response.raise_for_status() 
        print("Request successful:", response.json())
    except requests.exceptions.HTTPError as err:
        print(f"HTTP error occurred: {err}")
    except Exception as err:
        print(f"Other error occurred: {err}")



if __name__ == "__main__":
    selected_port = select_usb_port()
    if selected_port:
        serial_monitor(selected_port)
