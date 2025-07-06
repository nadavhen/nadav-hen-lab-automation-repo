
int MY_DELAY=500;
int PIN_NUM=13;
void setup() {
  // initialize digital pin LED as an output.
  pinMode(PIN_NUM, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PIN_NUM, HIGH);  // turn the LED on (HIGH is the voltage level)                   
  delayMicroseconds(MY_DELAY);// wait for a second
  digitalWrite(PIN_NUM, LOW);   // turn the LED off by making the voltage LOW                      
  delayMicroseconds(MY_DELAY);// wait for a second
}
