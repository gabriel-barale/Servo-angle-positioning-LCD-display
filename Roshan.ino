#include <ESP32Servo.h>  // Use the ESP32-specific Servo library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Define Pins
#define POT1_PIN    34  // Analog pin for Potentiometer 1
#define POT2_PIN    35  // Analog pin for Potentiometer 2
#define SERVO1_PIN  18  // PWM pin for SG90
#define SERVO2_PIN  19  // PWM pin for MG996R

// Initialize objects
Servo servo1;
Servo servo2;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set I2C address to 0x27 for a 16x2 display

void setup() {
  // Allow allocation of all timers for ESP32 Servos
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  
  // Attach servos with standard pulse width timings
  servo1.setPeriodHertz(50);    // Standard 50hz servo
  servo2.setPeriodHertz(50);
  
  servo1.attach(SERVO1_PIN, 500, 2400); 
  servo2.attach(SERVO2_PIN, 500, 2400);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Print static labels to prevent screen flickering
  lcd.setCursor(0, 0);
  lcd.print("S1 Deg: ");
  lcd.setCursor(0, 1);
  lcd.print("S2 Deg: ");
}

void loop() {
  // 1. Read Potentiometers (ESP32 ADC is 12-bit: 0 to 4095)
  int pot1Val = analogRead(POT1_PIN);
  int pot2Val = analogRead(POT2_PIN);

  // 2. Map values for the Servo hardware (0 to 180 degrees)
  int servo1Angle = map(pot1Val, 0, 4095, 0, 180);
  int servo2Angle = map(pot2Val, 0, 4095, 0, 180);

  // 3. Map values for your custom Display (-90 to 90 degrees)
  int displayAngle1 = map(pot1Val, 0, 4095, -90, 90);
  int displayAngle2 = map(pot2Val, 0, 4095, -90, 90);

  // 4. Move the Servos
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);

  // 5. Update the LCD Display
  // Servo 1 Display
  lcd.setCursor(8, 0);
  lcd.print("    "); // Clear old characters
  lcd.setCursor(8, 0);
  lcd.print(displayAngle1);

  // Servo 2 Display
  lcd.setCursor(8, 1);
  lcd.print("    "); // Clear old characters
  lcd.setCursor(8, 1);
  lcd.print(displayAngle2);

  delay(50); // Small delay to smooth out updates and prevent LCD stutter
}