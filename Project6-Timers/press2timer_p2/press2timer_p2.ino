#include <MsTimer2.h> // Include the MsTimer2 library for timer functionality

#define LED_PIN 4 // Define the pin number for the LED
#define BUTTON_PIN 6 // Define the pin number for the button
#define INTERRUPT_PIN 2 // Define the pin number for the interrupt (must be pin 2 or 3 on most Arduino boards)

volatile bool buttonPressed = false; // Flag to track the button state (not used in this code)
volatile unsigned long lastInterruptTime = 0; // Timestamp of the last interrupt (not used in this code)

// Interrupt service routine (ISR) to handle button press
void handleInterrupt() {
  digitalWrite(LED_PIN, HIGH); // Turn the LED on
  Serial.println("Button pressed: LED ON"); // Print a message to the Serial Monitor
  MsTimer2::start(); // Start the timer to turn off the LED after a specified period
}

// Function to turn off the LED (called by the timer)
void turn_off_light() {
  digitalWrite(LED_PIN, LOW); // Turn the LED off
}

void setup() {
  // Initialize the Serial communication for debugging
  Serial.begin(9600);

  // Set the timer to call `turn_off_light` after 30ms
  MsTimer2::set(30, turn_off_light);

  // Set pin modes
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configure the button pin as input with an internal pull-up resistor
  pinMode(LED_PIN, OUTPUT); // Configure the LED pin as output
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); // Configure the interrupt pin as input with an internal pull-up resistor

  // Attach an interrupt to the INTERRUPT_PIN
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, RISING);
  // The interrupt triggers when the signal on INTERRUPT_PIN changes from LOW to HIGH (button press)
}

void loop() {
  // Simulate some calculations in the main loop
  for (int i = 0; i < 10000; i++) {
    Serial.println("calculating..."); // Print a message to the Serial Monitor
  }
}
