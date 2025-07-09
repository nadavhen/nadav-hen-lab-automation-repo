import time
import threading
import FreeSimpleGUI as sg
from pymata4 import pymata4

# Initialize the board connection
board = pymata4.Pymata4(com_port="COM4")

# Define pin numbers
BUTTON_PIN = 6  # Pin connected to the button
LED_PIN = 4     # Pin connected to the LED

# Timer object to turn off the LED
led_timer = None  # Initialize the timer as None
timer_interval_ms = 2000  # Default timer interval in milliseconds

# GUI Layout
layout = [
    [sg.Text("Button State:"), sg.Text("Not Pressed", key="-BUTTON_STATE-", size=(15, 1))],  # Display button state
    [sg.Text("Timer Interval (milliseconds):"), sg.Input(default_text="2000", key="-TIMER_INPUT-", size=(10, 1))],  # Input for timer interval
    [sg.Button("Set Timer Interval", key="-SET_TIMER-")],  # Button to set the timer interval
    [sg.Multiline(size=(50, 10), key="-LOG-", disabled=True)],  # Log area to display actions
    [sg.Button("Exit")]  # Exit button
]

# Create the GUI window
window = sg.Window("Reacting to Button Press with Timer", layout)

def turn_off_led():
    """
    Turn off the LED and log the action.
    This function is called by the timer after the specified interval.
    """
    board.digital_write(LED_PIN, 0)  # Turn off the LED
    window["-LOG-"].print("LED turned off by timer.")  # Log the action in the GUI

def button_press(data):
    """
    Callback function triggered when the button state changes.
    Handles turning on the LED and starting the timer to turn it off.
    """
    global led_timer  # Access the global timer variable
    pin = data[1]  # Pin number (not used here but part of the callback data)
    value = data[2]  # Button state: 1 for pressed, 0 for released

    if value == 1:  # Button pressed
        window["-BUTTON_STATE-"].update("Pressed")  # Update button state in the GUI
        window["-LOG-"].print("Button pressed!")  # Log the button press
        board.digital_write(LED_PIN, 1)  # Turn on the LED

        # Cancel any existing timer to avoid overlapping timers
        if led_timer is not None:
            led_timer.cancel()

        # Start a new timer to turn off the LED after the specified interval (converted to seconds)
        led_timer = threading.Timer(timer_interval_ms / 1000.0, turn_off_led)
        led_timer.start()
    else:  # Button released
        window["-BUTTON_STATE-"].update("Not Pressed")  # Update button state in the GUI
        window["-LOG-"].print("Button released!")  # Log the button release

# Configure the button pin as input with pull-up and the LED pin as output
board.set_pin_mode_digital_input_pullup(pin_number=BUTTON_PIN, callback=button_press)  # Button pin setup
board.set_pin_mode_digital_output(LED_PIN)  # LED pin setup

try:
    print("Program is running. Use the GUI to interact.")
    while True:
        # Read GUI events
        event, values = window.read(timeout=100)  # Timeout ensures the program remains responsive
        if event == sg.WINDOW_CLOSED or event == "Exit":  # Exit condition
            break
        elif event == "-SET_TIMER-":  # Handle setting a new timer interval
            try:
                # Update the timer interval from the input field
                timer_interval_ms = int(values["-TIMER_INPUT-"])
                window["-LOG-"].print(f"Timer interval set to {timer_interval_ms} milliseconds.")
            except ValueError:
                # Handle invalid input
                window["-LOG-"].print("Invalid timer interval. Please enter a valid number.")

        time.sleep(0.1)  # Keep the program running without consuming too much CPU
except KeyboardInterrupt:
    # Handle program interruption (e.g., Ctrl+C)
    print("Shutting down...")
finally:
    # Clean up resources on exit
    if led_timer is not None:
        led_timer.cancel()  # Cancel the timer if it is running
    board.shutdown()  # Shutdown the board connection
    window.close()  # Close the GUI window