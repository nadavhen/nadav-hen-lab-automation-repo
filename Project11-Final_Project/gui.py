import serial
import threading
import csv
from datetime import datetime
import FreeSimpleGUI as sg
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

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
    # Clear the CSV file at the start of the program
    with open(r'C:\Users\itclass14\Documents\GitHub\nadav-hen-lab-automation-repo\Project11-Final_Project\serial_output.csv', mode='w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        csv_writer.writerow(['Timestamp', 'Angle', 'Buzzer'])  # Write the header

    stop_thread = threading.Event()  # Event to signal the thread to stop

    def read_serial():
        with open(r'C:\Users\itclass14\Documents\GitHub\nadav-hen-lab-automation-repo\Project11-Final_Project\serial_output.csv', mode='a', newline='') as csvfile:
            csv_writer = csv.writer(csvfile)
            while not stop_thread.is_set():  # Check if the thread should stop
                try:
                    response = serial_comm.read_response()
                    if response:
                        # Split the response into components (assuming it's comma-separated)
                        response_parts = response.split(',')
                        csv_writer.writerow(response_parts)  # Write the response parts directly to the CSV
                        csvfile.flush()  # Ensure data is written immediately
                except Exception as e:
                    print(f"Error in read_serial: {e}")
                    break

    def draw_figure(canvas, figure):
        figure_canvas_agg = FigureCanvasTkAgg(figure, canvas)
        figure_canvas_agg.draw()
        figure_canvas_agg.get_tk_widget().pack(side='top', fill='both', expand=1)
        return figure_canvas_agg

    def update_plot():
        try:
            # Read the CSV file
            data = pd.read_csv(r'C:\Users\itclass14\Documents\GitHub\nadav-hen-lab-automation-repo\Project11-Final_Project\serial_output.csv')
            timestamps = pd.to_datetime(data['Timestamp'], errors='coerce')
            angles = pd.to_numeric(data['Angle'], errors='coerce')

            # Drop rows with invalid data
            valid_data = data.dropna(subset=['Timestamp', 'Angle'])

            # Clear and redraw the plot
            ax.clear()
            ax.plot(valid_data['Timestamp'], valid_data['Angle'], label='Angle over Time')
            ax.set_xlabel('Timestamp')
            ax.set_ylabel('Angle')
            ax.set_title('Live Plot of Serial Responses')
            ax.legend()
            plt.xticks(rotation=45)
            fig.canvas.draw_idle()

            # Update the labels for the current angle and buzzer state
            if not valid_data.empty:
                current_angle = valid_data['Angle'].iloc[-1]
                current_buzzer = data['Buzzer'].iloc[-1]
                window['-ANGLE-'].update(f"Current Angle: {current_angle}")
                if current_buzzer == "ON":
                    window['-BUZZER-'].update(f"Buzzer State: {current_buzzer}", text_color="red")
                else:
                    window['-BUZZER-'].update(f"Buzzer State: {current_buzzer}", text_color="black")
        except Exception as e:
            print(f"Error updating plot: {e}")

    # Start a thread to continuously read from the serial port
    threading.Thread(target=read_serial, daemon=True).start()

    # Create the plot
    fig, ax = plt.subplots()

    # Define the layout
    layout = [
        [sg.Canvas(key='-CANVAS-')],
        [sg.Text("Current Angle: N/A", key='-ANGLE-', size=(20, 1), text_color="black", background_color="white", font=("Helvetica", 14)),
         sg.Text("Buzzer State: N/A", key='-BUZZER-', size=(20, 1), text_color="black", background_color="white", font=("Helvetica", 14))]
    ]

    # Create the window with a white background
    window = sg.Window("Serial Communication GUI", layout, finalize=True, background_color="white")

    # Embed the plot into the GUI
    canvas_elem = window['-CANVAS-']
    canvas = canvas_elem.Widget
    figure_canvas_agg = draw_figure(canvas, fig)

    # Event loop
    while True:
        event, values = window.read(timeout=1000)  # Update every second
        if event == sg.WINDOW_CLOSED:
            break

        # Update the plot
        update_plot()

    window.close()

def main():
    # Initialize serial communication
    serial_comm = SerialComm(port='COM4', baudrate=9600)  # Adjust port and baudrate as needed

    # Create and start the GUI
    create_gui(serial_comm)

if __name__ == "__main__":
    main()