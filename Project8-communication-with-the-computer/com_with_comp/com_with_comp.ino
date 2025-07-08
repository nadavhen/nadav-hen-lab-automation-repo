#include <MsTimer2.h> // Include the MsTimer2 library for timer functionality

#define LED_PIN 4 // Define the pin number for the LED
#define BUTTON_PIN 6 // Define the pin number for the button
#define INTERRUPT_PIN 2 // Define the pin number for the interrupt (must be pin 2 or 3 on most Arduino boards)

volatile bool buttonPressed = false; // Flag to track the button state (not used in this code)
unsigned long ledDuration = 30; // Default duration for the LED to stay on (in milliseconds)

// Interrupt service routine (ISR) to handle button press
void handleInterrupt() {
  MsTimer2::start();
  digitalWrite(LED_PIN, HIGH); // Turn the LED on
  buttonPressed = true; // Set the flag to indicate the button was pressed
  Serial.println("1"); // Print "1" to the Serial Monitor when the button is pressed
  // Start the timer to turn off the LED after the specified period
}

// Function to turn off the LED (called by the timer)
void turn_off_light() {
  digitalWrite(LED_PIN, LOW); // Turn the LED off
  MsTimer2::stop();
  buttonPressed = false; // Reset the button pressed flag
}

void setup() {
  // Initialize the Serial communication for debugging
  Serial.begin(9600);

  // Set the timer to call `turn_off_light` after the default duration
  MsTimer2::set(ledDuration, turn_off_light);

  // Set pin modes
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configure the button pin as input with an internal pull-up resistor
  pinMode(LED_PIN, OUTPUT); // Configure the LED pin as output
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); // Configure the interrupt pin as input with an internal pull-up resistor

  // Attach an interrupt to the INTERRUPT_PIN
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, RISING);
  // The interrupt triggers when the signal on INTERRUPT_PIN changes from LOW to HIGH (button press)
}

void loop() {
  // Check if data is available on the serial port
  if (buttonPressed) {
    Serial.println("2"); // Print "2" to the Serial Monitor when the LED is turned on
  }
  else {
    Serial.println("0"); // Print "0" to the Serial Monitor when the LED is off
  }
  if (Serial.available() > 0) {
    // Read the incoming data as a string
    String input = Serial.readStringUntil('\n'); // Wait for a newline character

    // Validate the input
    if (input.length() == 0) {
      Serial.println("Error: Received empty input.");
      return; // Exit the function if input is empty
    }

    // Attempt to convert the input to an integer
    unsigned long newDuration = input.toInt();

    // Check if the conversion was successful
    if (newDuration > 0) {
      ledDuration = newDuration;
      MsTimer2::set(ledDuration, turn_off_light); // Update the timer duration
      Serial.print("LED duration updated to: ");
      Serial.println(ledDuration);
    } else {
      Serial.println("Error: Invalid input. Please enter a positive number.");
    }
  }
}