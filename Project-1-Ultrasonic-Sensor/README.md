# Project 1: Distance Alarm System

## Overview
This project demonstrates basic ultrasonic distance measurement using the HC-SR04 sensor with Arduino. 
The system provides real-time feedback through an LCD display, LED brightness control, and distance-based buzzer alerts.

## Learning Outcomes
Through this project, I learned:
- How ultrasonic sensors measure distance using time-of-flight
- Pulse generation and measurement with Arduino
- I2C communication protocol for LCD displays
- Non-blocking code using `millis()` instead of `delay()`
- Tone generation for audio feedback
- Mapping sensor values to output ranges

## Components Required
- Arduino Uno (or compatible board)
- HC-SR04 Ultrasonic Sensor (4 pins) 
- 16x2 I2C LCD Display (address 0x27)
- LED (any color) 
- Buzzer (passive)
- Resistors (220Ω)
- Breadboard and jumper wires

## Circuit Connections
### LCD Display (I2C)
- GND → Arduino GND
- VCC → Arduino 5V
- SDA → Arduino A4
- SCL → Arduino A5

### HC-SR04 Sensor
- VCC → Arduino 5V
- GND → Arduino GND
- Trig → Arduino Pin 9
- Echo → Arduino Pin 10

### Other Components
- LED → Arduino Digital Pin 6 to anode leg (+) with 220Ω resistor from cathode (-) to GND
- Buzzer → Arduino Pin 8 to (+) and from opposite end (-) to GND

## How It Works
### Distance Measurement
The HC-SR04 sensor works by:
1. Sending a 10μs HIGH pulse to the Trig pin
2. Sensor emits 8 ultrasonic pulses at 40kHz
3. Echo pin goes HIGH when pulse is sent, LOW when echo returns
4. Distance = (pulse duration × 0.0343 cm/μs) / 2

The speed of sound is approximately 343 m/s or 0.0343 cm/μs. 
We divide by 2 because the sound travels to the object and back and we only want the "first" travel (distance) 

### Feedback Systems
**LCD Display:**
- Updates every 1 second (1000ms interval)
- Shows distance in centimeters

**LED Brightness:**
- Closer objects = brighter LED
- Distance mapped from 0-50cm range to 255-0 brightness (inverted)

**Buzzer Alerts:**
- Distance ≤ 10cm: 1000Hz tone (high pitch - danger zone)
- Distance 10-30cm: 800Hz tone (medium pitch - warning)
- Distance 30-60cm: 500Hz tone (low pitch - caution)
- Distance > 60cm: No sound
- Beep duration: 200ms on/off cycle
  
**Note:** The sensor may give unreliable readings for:
- Soft materials (cloth, foam)
- Angled surfaces
- Very small or thin objects
- Out of range items

## Code Features
- **Non-blocking timing:** Uses `millis()` for LCD updates and buzzer control
- **Distance range mapping:** LED brightness inversely proportional to distance
- **Multi-level alerts:** Different buzzer frequencies for distance zones
- **Efficient display updates:** LCD only refreshes once per second to reduce flicker

## Libraries Required
```cpp
#include <LiquidCrystal_I2C.h>
```
Can be found directly from this link: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
Download as zip file
Install via Arduino IDE: Sketch → Include Library -> Add .ZIP library → Downloads → Search for "Arduino-LiquidCrystal-I2C-library-master"

## License
This project is open source and available for educational purposes.
