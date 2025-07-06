int MY_DELAY = 1;
int counter = 0; // Declare a variable to store the result of the calculation

void setup() {
  // initialize digital pin LED as an output.
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  // Use PORT manipulation to turn both pins on simultaneously
  PORTB |= (1 << PB4) | (1 << PB5); // Set pins 12 and 13 HIGH
  counter += 1; // Perform a calculation (increment the counter variable)
  delay(MY_DELAY); // wait for a second

  // Use PORT manipulation to turn both pins off simultaneously
  PORTB &= ~((1 << PB4) | (1 << PB5)); // Set pins 12 and 13 LOW
  delay(MY_DELAY); // wait for a second
}
