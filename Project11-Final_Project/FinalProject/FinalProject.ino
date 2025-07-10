#include "LIS3DHTR.h" // Include the library for the LIS3DHTR accelerometer
#include <Servo.h>    // Include the library for controlling servos
#include <Wire.h>     // Include the library for I2C communication
#include <Arduino.h>  // Include the core Arduino library
#include <U8x8lib.h>  // Include the library for controlling the OLED display

// Define pin numbers for the fan, servo, and buzzer
#define FAN_PIN 6
#define SERVO_PIN 7
#define BUZZ_PIN 5
#define WIRE Wire // Alias for the I2C Wire object

// Create objects for the accelerometer, servo, and OLED display
LIS3DHTR<TwoWire> LIS;           // Accelerometer object using hardware I2C
Servo myservo;                   // Servo object for controlling the servo motor
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); // OLED display object

// Function to map a float value from one range to another
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  while (!Serial) {}; // Wait for the serial connection to be established

  // Initialize the LIS3DHTR accelerometer
  LIS.begin(WIRE, 0x19); // Start I2C communication with the accelerometer at address 0x19
  delay(100); // Small delay to ensure proper initialization
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ); // Set the accelerometer's data rate to 50Hz

  // Initialize the servo motor
  myservo.attach(SERVO_PIN); // Attach the servo to pin 7

  // Initialize the buzzer and fan pins as outputs
  pinMode(BUZZ_PIN, OUTPUT); // Set the buzzer pin as an output
  pinMode(FAN_PIN, OUTPUT);  // Set the fan pin as an output

  // Initialize the OLED display
  u8x8.begin(); // Start the OLED display
  u8x8.setFlipMode(1); // Flip the display orientation (if needed)
}

void loop() {
  // Check if the accelerometer is connected
  if (!LIS) {
    Serial.println("LIS3DHTR didn't connect."); // Print an error message if the accelerometer is not connected
    while (1); // Halt the program if the accelerometer is not connected
    return; // Exit the loop (though this is redundant due to the infinite loop above)
  }

  double accelerationY = LIS.getAccelerationY();
  int servoAngle = mapFloat(accelerationY, -1.0, 1.0, 0.0, 180.0);
  myservo.write(servoAngle); // Set the servo angle based on the Y-axis acceleration

  // Determine buzzer state
  String buzzerState = (servoAngle > 170 || servoAngle < 10) ? "ON" : "OFF";
  if (buzzerState == "ON") {
    analogWrite(BUZZ_PIN, 128); // Turn the buzzer on
    analogWrite(FAN_PIN, 0);
  } else {
    analogWrite(BUZZ_PIN, 0); // Turn the buzzer off
    analogWrite(FAN_PIN, 100);
  }

  // Log to Serial
  Serial.print(millis());
  Serial.print(",");
  Serial.print(servoAngle);
  Serial.print(",");
  Serial.println(buzzerState);

  // Display on OLED
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print("Angle: ");
  u8x8.print(String(servoAngle) + " ");
  u8x8.setCursor(0, 1);
  u8x8.print("Buzzer: ");
  u8x8.print(buzzerState+" ");

  delay(100); // Add a small delay for stability
}
