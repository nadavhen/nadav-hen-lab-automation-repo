import serial
import threading
import FreeSimpleGUI as sg

class SerialComm:
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate
        self.serial_connection = serial.Serial(port, baudrate, timeout=1)

    def send_data(self, data):
        self.serial_connection.write(data.encode())

    def read_response(self):
        return self.serial_connection.readline().decode().strip()

def create_gui(serial_comm):
    def read_serial():
        while True:
            try:
                response = serial_comm.read_response()
                if response:
                    window.write_event_value('-RESPONSE-', response)
            except Exception as e:
                window.write_event_value('-ERROR-', str(e))
                break

    # Start a thread to continuously read from the serial port
    threading.Thread(target=read_serial, daemon=True).start()

    # Define the layout
    layout = [
        [sg.Text("Enter a number:"), sg.Input(key='-INPUT-', size=(20, 1))],
        [sg.Button("Send", key='-SEND-')],
        [sg.Multiline(size=(50, 10), key='-OUTPUT-', disabled=True)]
    ]

    # Create the window
    window = sg.Window("Serial Communication GUI", layout)

    # Event loop
    while True:
        event, values = window.read()
        if event == sg.WINDOW_CLOSED:
            break
        elif event == '-SEND-':
            number = values['-INPUT-']
            if number.isdigit():
                serial_comm.send_data(number)
                window['-OUTPUT-'].print(f"Sent: {number}")
            else:
                window['-OUTPUT-'].print("Please enter a valid number.")
        elif event == '-RESPONSE-':
            window['-OUTPUT-'].print(values['-RESPONSE-'])
        elif event == '-ERROR-':
            window['-OUTPUT-'].print(f"Error: {values['-ERROR-']}")

    window.close()

def main():
    # Initialize serial communication
    serial_comm = SerialComm(port='COM4', baudrate=9600)  # Adjust port and baudrate as needed

    # Create and start the GUI
    create_gui(serial_comm)

if __name__ == "__main__":
    main()