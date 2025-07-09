#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int pos = 0;                 // variable to store the servo position
int fanPin = 6;              // PWM-enabled pin connected to the fan
int potentiometerPin = A0;   // Analog pin connected to the potentiometer

void setup() {
    Serial.begin(9600);       // Initialize serial communication   // Attach the servo on pin 3 to the servo object
    pinMode(fanPin, OUTPUT);  // Set the fan pin as an output
}

void loop() {
    // Read the potentiometer value (0–1023)
    int potValue = analogRead(potentiometerPin);

    // Map the potentiometer value to the PWM range (0–255)
    int fanSpeed = map(potValue, 0, 1023, 0, 255);

    // Set the fan speed using PWM
    analogWrite(fanPin, fanSpeed);

    // Print the potentiometer value and fan speed to the Serial Monitor
    Serial.print("Potentiometer Value: ");
    Serial.print(potValue);
    Serial.print(" | Fan Speed: ");
    Serial.println(fanSpeed);

    // Optional: Add a small delay for stability
    delay(50);
}