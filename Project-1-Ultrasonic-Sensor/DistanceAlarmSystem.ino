// First things first - we need the LCD library to talk to our screen
#include <LiquidCrystal_I2C.h>

// Screen object - 0x27 is the address,columns, rows
LiquidCrystal_I2C screen(0x27, 16, 2);

/* Hardware connections - this is also included in the README
  LCD Screen connections:
  GND -> GND
  VCC -> 5V 
  SDA -> A4 (data line for I2C)
  SCL -> A5 (clock line for I2C)
   
  HC-SR04 Ultrasonic Sensor:
  Trig -> ~9 (sends pulse)
  Echo -> 10 (receives pulse)
*/ 

// Pin definitions 
const int led = 6;          // LED 
const int buzzer = 8;       // Buzzer 
const int trigPin = 9;      // Sensor trigger pin
const int echoPin = 10;     // Sensor echo pin

// Variables for distance calculation
float duration, distance;  

// Timing variables - used to avoid use of delays
unsigned long lastPrint = 0;               // Tracks when I last updated the screen
unsigned long lastBeep = 0;                // Tracks when I last started a beep
const unsigned long beepInterval = 200;    // Beep duration (200ms)
const unsigned long timeInterval = 1000;   // Update the screen (1 second)

// Flag to track if the buzzer is ON
bool buzzing = false;

void setup() {
  // Initialize serial communication 
  Serial.begin(9600);
  
  // Initialize the LCD screen
  screen.begin();
  screen.setCursor(0, 0);  
  
  // Configure pins
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Grab the current time in milliseconds
  unsigned long now = millis();
  
  // DISTANCE MEASUREMENT
  // Okay here's how I measured distance 
  // First, make sure the trigger pin is LOW 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);               // Short pause to ensure a clean signal
  
  // Then send out a 10ms pulse 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Wait for the echo and measure duration to come back
  duration = pulseIn(echoPin, HIGH);  // Units of time are in μs
  
  // The speed of sound is approximately 343 m/s or 0.0343 cm/μs
  // Multiply to cancel out units
  // Lastly divide by 2 because the sound travels to the object and back and we only want the "first" travel (distance)
  distance = (duration * 0.0343) / 2;
  
  // LCD SCREEN PART
  // Only update the screen every second to avoid flickering
  if(now - lastPrint >= timeInterval) {
    screen.setCursor(0, 0);          
    screen.print("Distance:       "); // Print label (extra spaces clears old digits)
    screen.setCursor(10, 0);         
    screen.print(distance);          
    lastPrint = now;                  // Update printing time
  }
  
  //  LED BRIGHTNESS PART
  // I made the LED brighter as objects get closer 
  // Use map() to convert distance (0-50cm range) to brightness (255-0)
  int brightness = map(distance, 0, 50, 255, 0);
  analogWrite(led, brightness);       // Set the LED brightness using PWM (any pin with ~)
  
  // BUZZER ALARM PART
  // Different beep tones based on how close something is
  // Really close (10cm or less) -> HIGH ALERT -> 1000 Hz tone
  if(distance <= 10) {
    // Only start a new beep if we're not already beeping
    if(!buzzing) {  
      tone(buzzer, 1000);             // High pitch warning
      buzzing = true;
      lastBeep = now;
    }
  } 
  // Medium distance (10-30cm) -> moderate warning -> 800 Hz tone
  else if(distance > 10 && distance <= 30) {
    if(!buzzing) {
      tone(buzzer, 800);              // Medium pitch
      buzzing = true;
      lastBeep = now;
    }
  } 
  // Further away (30-60cm) -> gentle warning -> 500 Hz tone
  else if(distance > 30 && distance < 60) {
    if(!buzzing) {
      tone(buzzer, 500);              // Lower pitch
      buzzing = true;
      lastBeep = now;
    }
  }
  
  // Turn off the buzzer after the beep interval has passed
  // This creates the beeping pattern instead of continuous sound
  if(buzzing && (now - lastBeep >= beepInterval)) {
    noTone(buzzer);                   // Stop the tone
    buzzing = false;                  // Reset our flag so we can beep again
  }
}