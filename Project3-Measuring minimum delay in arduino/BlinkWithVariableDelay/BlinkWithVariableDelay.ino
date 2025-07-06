
int MY_DELAY=1;
int PIN_NUM=4;
void setup() {
  // initialize digital pin LED as an output.
  pinMode(PIN_NUM, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PIN_NUM, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(MY_DELAY);                      // wait for a second
  digitalWrite(PIN_NUM, LOW);   // turn the LED off by making the voltage LOW
  delay(MY_DELAY);                      // wait for a second
}
