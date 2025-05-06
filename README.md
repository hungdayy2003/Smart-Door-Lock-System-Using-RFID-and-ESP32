# Smart Door Lock with RFID and Keypad

This is a smart door lock system using RFID cards and a keypad for secure access control. The system authenticates users via RFID and a 4-digit PIN, then updates access logs to Firebase.

## Features
- RFID-based user recognition
- Keypad PIN entry
- Servo-controlled lock mechanism
- Real-time data logging to Firebase
- LCD feedback for user interaction

## Hardware Required
- ESP32
- MFRC522 RFID module
- 4x4 Keypad
- Servo motor
- LCD I2C display
- WiFi connection

## Setup
1. Connect components as per the wiring in the code.
2. Replace WiFi SSID, password, and Firebase URL in the code.
3. Upload the sketch to ESP32.
4. Monitor serial output for logs and debugging.
