#include "LIS3DHTR.h"
#include <Servo.h>
#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>

#define FAN_PIN 6
#define SERVO_PIN 7
#define BUZZ_PIN 5
#define WIRE Wire

LIS3DHTR<TwoWire> LIS;           //Hardware I2C
Servo myservo; // Create a Servo object for pin 7
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {};
  LIS.begin(WIRE, 0x19); // IIC init
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);

  myservo.attach(SERVO_PIN); // Attach the servo on pin 7 to the servo object
  pinMode(BUZZ_PIN, OUTPUT); // Set the buzzer pin as an output
  pinMode(FAN_PIN, OUTPUT); // Set the fan pin as an output

  // Initialize the OLED display
  u8x8.begin();
  u8x8.setFlipMode(1);
}

void loop() {
  if (!LIS) {
    Serial.println("LIS3DHTR didn't connect.");
    while (1);
    return;
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
