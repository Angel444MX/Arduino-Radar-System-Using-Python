#include <LiquidCrystal_I2C.h>    // I2C LCD library 
#include <Servo.h>                // Include the library to control servo motor

LiquidCrystal_I2C Screen(0x27, 16, 2);  // Create an LCD object
Servo Motor;                            // Create a Servo object named --> Motor

// Pin definitions
const int motorPin = 9;                   // Pin connected to servo signal wire
int currentAngle = 90;                    // Store the current angle of servo --> starts at 90 degrees
unsigned long lastPrint = 0;              // Variable for last time the LCD was updated
const unsigned long printInterval = 500;  // Time interval between LCD updates

void setup() {
  Serial.begin(9600);           // Start serial communication --> info will be shown in screen --> Serial Monitor
  Screen.begin();               // Initialize the LCD display

  Motor.attach(motorPin);       // Attach the servo motor to pin 9
  Motor.write(currentAngle);    // Move the servo to starting position

  Serial.println("Servo Angle Controller ");    
  Serial.println("Enter angle (0 - 180)");      

  Screen.setCursor(0, 0);     
  Screen.print("Angle:");
  Screen.setCursor(0, 1);
  Screen.print(currentAngle);
}

void loop () {
  unsigned long now = millis();               // Grab the current time in milliseconds

  // Check if data has been entered into the Serial Monitor
  if(Serial.available() > 0) {
    int targetAngle = Serial.parseInt();      // Read an integer entered by the user

    // Verify that the entered angle is between 0 and 180 degrees
    if(targetAngle >= 0 && targetAngle <= 180) {
      MoveServo(currentAngle, targetAngle);   // Move the servo smoothly from the current angle to the target angle
      currentAngle = targetAngle;             // Save the new angle as the current angle
      Serial.print("Moved to: ");
      Serial.println(currentAngle);
    }
  }

  // Check if 500ms have passed since last LCD update
  if(now - lastPrint >= printInterval) {
    Screen.setCursor(0, 1);
    Screen.print("    ");         // Clear the previous number by printing spaces
    Screen.setCursor(0, 1);       // Return the cursor to the beginning of the second row
    Screen.print(currentAngle);   // Display the current servo angle
    lastPrint = now;              // Save the current time as the last LCD update time
  }
}

void MoveServo(int fromAngle, int toAngle) {

  // Check if the servo needs to rotate in increasing direction
  if(fromAngle < toAngle) {

    // Increase angle one degree at a time
    for(int position = fromAngle; position <= toAngle; position++) {
      Motor.write(position);    // Move the servo to current position
      delay(15);                // Wait 15ms before moving to the next position
    }

  } else {
    // Otherwise rotate in decreasing direction
    // Decrease the angle one degree at a time
    for(int position = fromAngle; position >= toAngle; position--) {
      Motor.write(position);    // Move the servo to the current position
      delay(15);
    }
  }
}