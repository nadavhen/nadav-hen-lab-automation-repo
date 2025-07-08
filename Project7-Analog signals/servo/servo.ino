#include <MD_PWM.h> // Include the MD_PWM library

const int potentiometerPin = A0; // Define the potentiometer pin
const int pwmPin = 4;           // Define the PWM output pin
const uint16_t PWM_FREQ = 50;   // Set PWM frequency in Hz

MD_PWM pwmController(pwmPin);   // Create an MD_PWM object for pin 4

void setup() {
  Serial.begin(9600);           // Initialize serial communication

  // Initialize the PWM controller with the specified frequency
  if (!pwmController.begin(PWM_FREQ)) {
    Serial.println("Failed to initialize PWM on pin 4");
    while (true); // Halt execution if initialization fails
  }
}

void loop() {
  int potValue = analogRead(potentiometerPin); // Read the potentiometer value
  int pwmValue = map(potValue, 0, 1023, 0, 255); // Map the value to PWM range
  pwmController.write(pwmValue);               // Set the PWM duty cycle on pin 4
  Serial.println(potValue);                    // Print the PWM value to the Serial Monitor
  delay(50);                                   // Add a small delay
}