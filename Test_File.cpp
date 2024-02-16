#include <DHT.h>
#include <TimeLib.h>
#include <SdFat.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define SD_FAT_TYPE 1
#define SD_CS_PIN 23
SdFat sd;
File32 myFile;

const int pumpRelayOne = 7;  // Define the pin connected to the relay
const int pumpRelayTwo = 13; // Define the pin connected to the relay

unsigned long previousMorningMillis = 0; // Variable to store the last time the pumps were run
unsigned long previousEveningMillis = 0; // Variable to store the last time checkTemps was called in the evening

const long morningInterval = 172800000; // 2 days in milliseconds
const long eveningInterval = 28800000;  // 8 hours in milliseconds

int daysCounter = 0; // Starting count for the number of days
bool stopProgram = false;  // Variable to control program exit

void setup()
{
  Serial.begin(115200);      // Start serial communication
  setTime(12, 0, 0, 2, 16, 2024); // Set initial date and time (replace these values with your desired date and time)

  pinMode(pumpRelayOne, OUTPUT);
  pinMode(pumpRelayTwo, OUTPUT);
  digitalWrite(pumpRelayOne, LOW);
  digitalWrite(pumpRelayTwo, LOW);
  dht.begin();             // Sensor Start

  // Barebones SD initialization
  if (!sd.begin(SD_CS_PIN))
  {
    Serial.println(F("SD initialization failed!"));
    exit(EXIT_FAILURE);
  }

  // Log initial temperature and humidity check
  float initialHumidity = dht.readHumidity();
  float initialTemperature = dht.readTemperature(true);
  logData("Initial Check", initialHumidity, initialTemperature);
}

void loop()
{
  unsigned long currentMillis = millis(); // Get the current time
  float h = dht.readHumidity();          // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  float f = dht.readTemperature(true);   // Read temperature as Fahrenheit (isFahrenheit = true)

  // Check if it's noon to start the temperature and humidity checks
  if (hour() == 12 && minute() == 0 && second() == 0)
  {
    previousEveningMillis = currentMillis; // Save the current time for the evening check
    checkTemps(h, f);                      // Run the checkTemps function at noon
  }

  // Check if two days have passed after noon
  if (currentMillis - previousMorningMillis >= morningInterval && hour() == 12)
  {
    previousMorningMillis = currentMillis; // Save the current time
    runPumps();                            // Run the pumps function
    daysCounter += 2;                      // Increment the days counter by 2 since we want to run pumps every two days

    // Check if 7 days have passed
    if (daysCounter >= 7)
    {
      Serial.println("Program will now end and shut down gracefully.");
      exit(EXIT_SUCCESS);  // Use exit() to end the program gracefully
    }
  }

  // Check if it's time for the evening check
  if (currentMillis - previousEveningMillis >= eveningInterval)
  {
    previousEveningMillis = currentMillis; // Save the current time for the evening check
    checkTemps(h, f);                      // Run the checkTemps function in the evening
  }
}

void runPumps()
{
  digitalWrite(pumpRelayOne, HIGH);
  digitalWrite(pumpRelayTwo, HIGH);

  // Read temperature and humidity
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  // Save data to SD card
  myFile = sd.open("data.txt", FILE_WRITE);
  if (myFile)
  {
    myFile.print("Pump Run: ");
    myFile.print(h);
    myFile.print("%, ");
    myFile.print("Temp: ");
    myFile.println(f);
    myFile.close();
  }
  else
  {
    Serial.println(F("Error opening file for writing"));
  }

  delay(20000);

  digitalWrite(pumpRelayOne, LOW);  // Turn off the pumps
  digitalWrite(pumpRelayTwo, LOW);

  // Calculate the time for the next pump run (18 hours later)
  setTime(hour() + 18, minute(), second(), day(), month(), year());

  // Save the current time for the next evening check
  previousEveningMillis = millis();
}


void checkTemps(float humidity, float temperature)
{
  // Serial.print("Evening - ");
  // Serial.print(humidity);
  // Serial.print("% ");
  // Serial.print(temperature);
  // Serial.println(F(" °F"));

  // Save data to SD card
  myFile = sd.open("data.txt", FILE_WRITE);
  if (myFile)
  {
    myFile.print("2pm: ");
    myFile.print(humidity);
    myFile.print("%, ");
    myFile.print("Temp: ");
    myFile.println(temperature);
    myFile.println(); // Add a newline for clarity

    // Save data for the second evening check
    float secondHumidity = dht.readHumidity();
    float secondTemperature = dht.readTemperature(true);
    
    myFile.print("10pm: ");
    myFile.print(secondHumidity);
    myFile.print("%, ");
    myFile.print("Temp: ");
    myFile.println(secondTemperature);
    
    myFile.close();
  }
  else
  {
    Serial.println(F("Error opening file for writing"));
  }
}

void logData(const char *timestamp, float humidity, float temperature)
{
  myFile = sd.open("data.txt", FILE_WRITE);
  if (myFile)
  {
    myFile.print(timestamp);
    myFile.print(": ");
    myFile.print(humidity);
    myFile.print("%, ");
    myFile.print("Temp: ");
    myFile.println(temperature);
    myFile.close();
  }
  else
  {
    Serial.println(F("Error opening file for writing"));
  }
}
