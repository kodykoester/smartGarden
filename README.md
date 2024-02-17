# Automated Greenhouse System

This Arduino code is designed for an automated greenhouse system that monitors temperature and humidity using a DHT22 sensor and logs the data to an SD card. Additionally, the system controls water pumps to irrigate the plants based on specific intervals.

## Components Used
- DHT22 sensor for temperature and humidity readings
- SdFat library for SD card interfacing
- Two relays for controlling water pumps
- Arduino board with SD card slot

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

1. **Temperature and Humidity Checks (8hrs)**
   - Every 8 hours, the system performs temperature and humidity checks and logs the data to the SD card.

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
2. Upload the code to your Arduino board.
3. Monitor the greenhouse environment using the logged data on the SD card.
