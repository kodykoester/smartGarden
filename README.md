# Metro RP2040 Garden Automation

This minimalistic Arduino project utilizes the Adafruit Metro RP2040 to automate garden care tasks based on environmental conditions. The code includes functionalities for reading temperature and humidity using a DHT22 sensor, running water pumps at specific intervals, and logging data to an SD card.

## Dependencies
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- [SdFat library](https://github.com/greiman/SdFat)

## Hardware Requirements
- DHT22 sensor connected to pin 2
- Two relays connected to pins 7 and 13 for controlling water pumps
- SD card module connected to pin 23

## Functionality
- The program runs water pumps once a day in the morning and performs an evening temperature check every 8 hours.
- Data, including pump run details and evening temperature readings, is logged to an SD card in "data.txt".

**Tested on Adafruit Metro RP2040.**

