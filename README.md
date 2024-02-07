# README

## Introduction
This project utilizes an ESP32 microcontroller to detect objects using an ultrasonic sensor and send the distance measurements to a Firebase Realtime Database. The system also connects to a WiFi network to enable data transmission to the cloud. This README provides an overview of the project's functionality and setup instructions.

## Requirements
- Arduino IDE or compatible IDE
- ESP32 development board
- Ultrasonic sensor (HC-SR04)
- WiFi network with internet access
- Firebase account with a Realtime Database

## Setup
1. **Hardware Setup**:
   - Connect the HC-SR04 ultrasonic sensor to the ESP32 as follows:
     - VCC to 5V
     - GND to GND
     - Trig to D2
     - Echo to D3

2. **Software Setup**:
   - Install the required libraries using the Arduino Library Manager:
     - WiFi.h
     - Firebase_ESP_Client.h
   - Include the necessary Firebase add-ons: `TokenHelper.h` and `RTDBHelper.h`

3. **Configuration**:
   - Update the following constants in the code with your WiFi network credentials and Firebase database details:
     - `ssid`: Your WiFi network SSID
     - `password`: Your WiFi network password
     - `DATABASE_URL`: Your Firebase Realtime Database URL
     - `API_KEY`: Your Firebase API key

## Usage
1. Upload the code to your ESP32 development board using the Arduino IDE or compatible IDE.
2. Open the serial monitor to view the output.
3. The system will connect to the WiFi network and initialize Firebase.
4. The ultrasonic sensor will continuously measure the distance to any object.
5. If an object is detected within 30cm, the distance measurement will be sent to the Firebase Realtime Database.
6. The ESP32 will then enter deep sleep mode for power saving.
7. Repeat steps 4-6 for continuous monitoring.

## Notes
- Ensure that the ESP32 is powered properly to maintain continuous operation.
- Adjust the distance threshold (`30cm`) in the `sendDataToFirebase` function according to your requirements.
- Monitor the serial output for debugging information and status updates.

## Troubleshooting
- If you encounter issues with WiFi connection or Firebase authentication, double-check your network credentials and Firebase configuration.
- Ensure proper wiring of the ultrasonic sensor and the ESP32 board.
- Check for any error messages in the serial monitor and refer to the respective sections of the code for troubleshooting.

## License
This project is licensed under the [MIT License](LICENSE). Feel free to modify and distribute it as per the terms of the license.
