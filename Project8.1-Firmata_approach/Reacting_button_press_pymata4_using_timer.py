import time
import threading
import FreeSimpleGUI as sg
from pymata4 import pymata4

board = pymata4.Pymata4(com_port="COM4")

BUTTON_PIN = 6
LED_PIN = 4

# Timer object to turn off the LED
led_timer = None
timer_interval_ms = 2000  # Default timer interval in milliseconds

# GUI Layout
layout = [
    [sg.Text("Button State:"), sg.Text("Not Pressed", key="-BUTTON_STATE-", size=(15, 1))],
    [sg.Text("Timer Interval (milliseconds):"), sg.Input(default_text="2000", key="-TIMER_INPUT-", size=(10, 1))],
    [sg.Button("Set Timer Interval", key="-SET_TIMER-")],
    [sg.Multiline(size=(50, 10), key="-LOG-", disabled=True)],
    [sg.Button("Exit")]
]

# Create the GUI window
window = sg.Window("Reacting to Button Press with Timer", layout)

def turn_off_led():
    """Turn off the LED."""
    board.digital_write(LED_PIN, 0)
    window["-LOG-"].print("LED turned off by timer.")

def button_press(data):
    global led_timer
    pin = data[1]
    value = data[2]
    if value == 1:  # Button pressed
        window["-BUTTON_STATE-"].update("Pressed")
        window["-LOG-"].print("Button pressed!")
        board.digital_write(LED_PIN, 1)  # Turn on the LED

        # Cancel any existing timer
        if led_timer is not None:
            led_timer.cancel()

        # Start a new timer to turn off the LED after the specified interval (converted to seconds)
        led_timer = threading.Timer(timer_interval_ms / 1000.0, turn_off_led)
        led_timer.start()
    else:  # Button released
        window["-BUTTON_STATE-"].update("Not Pressed")
        window["-LOG-"].print("Button released!")

# Configure the button pin as input with pull-up and the LED pin as output
board.set_pin_mode_digital_input_pullup(pin_number=BUTTON_PIN, callback=button_press)
board.set_pin_mode_digital_output(LED_PIN)

try:
    print("Program is running. Use the GUI to interact.")
    while True:
        event, values = window.read(timeout=100)  # Read GUI events
        if event == sg.WINDOW_CLOSED or event == "Exit":
            break
        elif event == "-SET_TIMER-":
            try:
                timer_interval_ms = int(values["-TIMER_INPUT-"])
                window["-LOG-"].print(f"Timer interval set to {timer_interval_ms} milliseconds.")
            except ValueError:
                window["-LOG-"].print("Invalid timer interval. Please enter a valid number.")

        time.sleep(0.1)  # Keep the program running
except KeyboardInterrupt:
    print("Shutting down...")
finally:
    if led_timer is not None:
        led_timer.cancel()  # Cancel the timer if it is running
    board.shutdown()
    window.close()