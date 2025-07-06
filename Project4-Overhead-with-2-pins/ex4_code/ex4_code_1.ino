int MY_DELAY = 1;
int counter = 0; // Declare a variable to store the result of the calculation

void setup() {
  // initialize digital pin LED as an output.
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);  // turn the LED on (HIGH is the voltage level)  
  counter += 1; // Perform a calculation (increment the counter variable)  
  digitalWrite(12, HIGH);               
  delay(MY_DELAY); // wait for a second
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW                      
  delay(MY_DELAY); // wait for a second
}