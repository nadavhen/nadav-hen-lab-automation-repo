#define LED_PIN 4
#define BUTTON_PIN 6
#define INTERRUPT_PIN 2 // Interrupt pin (must be pin 2 or 3 on most Arduino boards)

volatile bool buttonPressed = false; // Flag to track button state
volatile unsigned long lastInterruptTime = 0; // Timestamp of the last interrupt

void handleInterrupt() {
  digitalWrite(LED_PIN, HIGH); // Turn the LED on
  Serial.println("Button pressed: LED ON");
  lastInterruptTime = millis(); // Get the current time
}

void setup() {
  // Initialize the Serial communication
  Serial.begin(9600);

  // Set pin modes
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button input with internal pull-up
  pinMode(LED_PIN, OUTPUT); // LED output
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); // Interrupt pin input (connected to BUTTON_PIN)

  // Attach interrupt to INTERRUPT_PIN
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, HIGH);
  // The interrupt triggers on any change (HIGH to LOW or LOW to HIGH) on INTERRUPT_PIN
}

void loop() {
  if (millis() - lastInterruptTime > 5000) { // Check if 5 seconds have passed
    digitalWrite(LED_PIN, LOW); // Turn the LED off
    Serial.println("5 seconds passed: LED OFF");
    lastInterruptTime = millis(); // Reset the timer
  }
}