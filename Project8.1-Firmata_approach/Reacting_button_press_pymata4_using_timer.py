import time
import sys
from pymata4 import pymata4

board = pymata4.Pymata4(com_port="COM4") 

BUTTON_PIN = 6
LED_PIN = 4

def button_press(data):
    pin = data[1]
    value = data[2]
    if value == 1:  # Button pressed
        print("Button pressed!")
        board.digital_write(LED_PIN, 1)  # Turn on the LED
    else:  # Button released
        print("Button released!")
        board.digital_write(LED_PIN, 0)  # Turn off the LED

board.set_pin_mode_digital_input_pullup(pin_number=BUTTON_PIN, callback=button_press)
board.set_pin_mode_digital_output(LED_PIN)

try:
    print("Program is running. Press Ctrl+C to exit.")
    while True:
        time.sleep(0.1)  # Keep the program running
except KeyboardInterrupt:
    print("Shutting down...")
    board.shutdown()