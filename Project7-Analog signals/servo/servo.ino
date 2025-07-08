#include <Servo.h> // Include the Servo library

const int potentiometerPin = A0; // Define the potentiometer pin
const int servoPin = 7;           // Define the PWM output pin


Servo myservo;// Create a Servo object for pin 7

void setup() {
  Serial.begin(9600); 
  myservo.attach(servoPin);          // Initialize serial communication
}

void loop() {
  int potValue = analogRead(potentiometerPin); // Read the potentiometer value
  int angValue = map(potValue, 0, 1023, 0, 170); // Map the value to servo angle range
  myservo.write(angValue);               // Set the PWM duty cycle on pin 7
  Serial.println(potValue);                    // Print the PWM value to the Serial Monitor
  delay(50);                                   // Add a small delay
}