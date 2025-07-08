import serial
import threading
import tkinter as tk
from tkinter import scrolledtext

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
    def send_data():
        number = input_field.get()
        if number.isdigit():
            serial_comm.send_data(number)
            output_area.insert(tk.END, f"Sent: {number}\n")
        else:
            output_area.insert(tk.END, "Please enter a valid number.\n")

    def read_serial():
        while True:
            try:
                response = serial_comm.read_response()
                if response:
                    output_area.insert(tk.END, f"{response}\n")
            except Exception as e:
                output_area.insert(tk.END, f"Error: {e}\n")
                break

    # Start a thread to continuously read from the serial port
    threading.Thread(target=read_serial, daemon=True).start()

    # Create the main window
    root = tk.Tk()
    root.title("Serial Communication GUI")

    # Input field
    tk.Label(root, text="Enter a number:").pack()
    input_field = tk.Entry(root)
    input_field.pack()

    # Send button
    send_button = tk.Button(root, text="Send", command=send_data)
    send_button.pack()

    # Output area
    output_area = scrolledtext.ScrolledText(root, width=50, height=10)
    output_area.pack()

    # Start the GUI event loop
    root.mainloop()

def main():
    # Initialize serial communication
    serial_comm = SerialComm(port='COM4', baudrate=9600)  # Adjust port and baudrate as needed

    # Create and start the GUI
    create_gui(serial_comm)

if __name__ == "__main__":
    main()