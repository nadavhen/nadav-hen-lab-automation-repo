#define LED_PIN 4
#define BUTTON_PIN 6
void setup() {
  // Initialize the Serial communication
  Serial.begin(9600);

  // Set pin modes
  pinMode(BUTTON_PIN, INPUT); // Button input
  pinMode(LED_PIN, OUTPUT); // LED output
}

void loop() {
  // Read the state of the input pin (pin 6)
  int buttonState = digitalRead(BUTTON_PIN);

  // Check if the button is pressed (HIGH state)
  if (buttonState == HIGH) {
    digitalWrite(LED_PIN, HIGH); // Turn the LED on
    Serial.println("Button pressed: LED ON"); // Debug message for button press
  } else {
    digitalWrite(LED_PIN, LOW); // Turn the LED off
    Serial.println("Button released: LED OFF"); // Debug message for button release
  }

  for (int i = 0; i< 1000; i++){
    Serial.println("calculating...");
}
}