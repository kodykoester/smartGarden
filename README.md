# Automated Greenhouse System

This Arduino code is designed for an automated greenhouse system that monitors temperature and humidity using a DHT22 sensor and logs the data to an SD card. Additionally, the system controls water pumps to irrigate the plants based on specific intervals.

**Current Board:** Adafruit Metro RP2040

## Components Used
- DHT22 sensor for temperature and humidity readings
- SdFat library for SD card interfacing
- Two relays for controlling water pumps
- Adafruit Metro RP2040 board

## Wiring
- DHT22 sensor connected to pin 2
- SD card connected to pin 23
- Relay for Pump 1 connected to pin 7
- Relay for Pump 2 connected to pin 13

## Functionality

### Sensor Initialization
The code initializes the DHT22 sensor and the SD card, logging the initial temperature and humidity readings to the SD card.

### Main Loop
The main loop continuously monitors the current time and performs the following actions:

1. **Temperature and Humidity Checks (Noon)**
   - At noon, the system performs temperature and humidity checks and logs the data to the SD card.

2. **Pump Activation (Every 2 Days)**
   - Every two days, the water pumps are activated, and the corresponding data is logged to the SD card.

3. **Program Exit (After 8 Days)**
   - If eight days have passed, the program gracefully exits, shutting down the system.

4. **Evening Temperature and Humidity Checks**
   - In the evening, the system performs additional temperature and humidity checks and logs the data to the SD card.

### Functions
- `runPumps()`: Activates water pumps, logs data, and calculates the time for the next pump run.
- `checkTemps(float humidity, float temperature)`: Performs evening temperature and humidity checks and logs the data.
- `logData(const char *timestamp, float humidity, float temperature)`: Logs timestamped temperature and humidity data.

## Usage
1. Connect the components as per the provided wiring.
2. Upload the code to your Adafruit Metro RP2040 board.
3. Monitor the greenhouse environment using the logged data on the SD card.

## Future Scope
- **Photoresistors for Light Intensity Measurements:**
  - Enhance the system with photoresistors to measure precise light intensity for better plant care.

- **Raspberry Pi Integration for Web Application UI and Remote Control Access:**
  - Incorporate a Raspberry Pi for creating a web application UI and enabling remote control access to the greenhouse system.

- **MongoDB Connection for Cloud Data Storage:**
  - Implement MongoDB connection to enable cloud data storage for comprehensive analysis and monitoring.
