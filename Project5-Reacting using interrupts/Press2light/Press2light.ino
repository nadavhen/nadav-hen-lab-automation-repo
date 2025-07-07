#define LED_PIN 4
#define BUTTON_PIN 6
#define INTERRUPT_PIN 2 // Interrupt pin (must be pin 2 or 3 on most Arduino boards)

volatile bool buttonPressed = false; // Flag to track button state
volatile unsigned long lastInterruptTime = 0; // Timestamp of the last interrupt

void handleInterrupt() {
  // Interrupt service routine (ISR)
  unsigned long currentTime = millis(); // Get the current time
  if (currentTime - lastInterruptTime > 50) { // Check if 50ms have passed since the last interrupt
    buttonPressed = digitalRead(BUTTON_PIN); // Read the button state directly
    if (buttonPressed) {
      digitalWrite(LED_PIN, HIGH); // Turn the LED on
      Serial.println("Button pressed: LED ON");
    } else {
      digitalWrite(LED_PIN, LOW); // Turn the LED off
      Serial.println("Button released: LED OFF");
    }
    lastInterruptTime = currentTime; // Update the last interrupt time
  }
}

void setup() {
  // Initialize the Serial communication
  Serial.begin(9600);

  // Set pin modes
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button input with internal pull-up
  pinMode(LED_PIN, OUTPUT); // LED output
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); // Interrupt pin input (connected to BUTTON_PIN)

  // Attach interrupt to INTERRUPT_PIN
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, CHANGE);
  // The interrupt triggers on any change (HIGH to LOW or LOW to HIGH) on INTERRUPT_PIN
}

void loop() {
  for (int i = 0; i< 10000; i++){
      Serial.println("calculating...");
  }
}