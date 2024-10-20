import serial
import keyboard
import time

pressed_keys = {}

key_mapping = {
    "6": "left",
    "5": "right",
    "4": "up",
    "1": "down",
    "0": "enter",
    "2": "a",
    "3": "x",
}

def handle_serial_input(message):
    if ":" not in message:
        return
    button_index, action = message.split(":")
    
    if button_index in key_mapping:
        key = key_mapping[button_index]

        if action == "DOWN" and key not in pressed_keys:
            # Start pressing the key
            pressed_keys[key] = True
            print(f"Pressed {key}")
            
        elif action == "UP" and key in pressed_keys:
            # Release the key
            keyboard.release(key)
            del pressed_keys[key]
            print(f"Released {key}")

        print(f"Pressed keys: {list(pressed_keys.keys())}")

def open_serial_connection(port, baudrate):
    while True:
        try:
            ser = serial.Serial(port, baudrate)
            print("Serial connection established")
            return ser
        except serial.SerialException as e:
            print(f"Failed to connect to {port}: {e}")
            print("Retrying in 5 seconds...")
            time.sleep(5)

def main():
    # Open the serial port. Adjust the port name and baud rate to match your setup.
    port = 'COM7'
    baudrate = 115200
    ser = open_serial_connection(port, baudrate)

    try:
        while True:
            try:
                # Check for incoming serial messages
                if ser.in_waiting > 0:
                    message = ser.readline().decode('utf-8').strip()
                    print(f"Received: {message}")
                    handle_serial_input(message)

                # Simulate continuous keypresses for held keys
                for key in pressed_keys:
                    keyboard.press(key)  # Simulate repeated keypresses
                time.sleep(0.05)  # Adjust the repeat speed as needed

            except serial.SerialException as e:
                print(f"Serial error: {e}")
                ser.close()  # Close the connection
                print("Reconnecting...")
                ser = open_serial_connection(port, baudrate)

    except KeyboardInterrupt:
        ser.close()
        print("Serial connection closed")

if __name__ == "__main__":
    main()


aaaa