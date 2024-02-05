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

const long morningInterval = 86400000; // 24 hours in milliseconds
const long eveningInterval = 28800000; // 8 hours in milliseconds

int daysCounter = 0; // Starting count for the number of days
bool stopProgram = false;  // Variable to control program exit

void setup()
{
  Serial.begin(115200);      // Start serial communication
  setTime(6, 0, 0, 2, 23, 2024); // Set initial date and time (replace these values with your desired date and time)

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
}

void loop()
{
  unsigned long currentMillis = millis(); // Get the current time
  float h = dht.readHumidity();          // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  float f = dht.readTemperature(true);   // Read temperature as Fahrenheit (isFahrenheit = true)

  // Check if a day has passed
  if (currentMillis - previousMorningMillis >= morningInterval)
  {
    previousMorningMillis = currentMillis; // Save the current time
    runPumps();                            // Run the pumps function
    daysCounter++;                        // Increment the days counter

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
  Serial.println("Pumps running!");

  // Read temperature and humidity
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  // Save data to SD card
  myFile = sd.open("data.txt", FILE_WRITE);
  if (myFile)
  {
    myFile.print("Pump Run - ");
    myFile.print("Humidity: ");
    myFile.print(h);
    myFile.print("%, ");
    myFile.print("Temperature: ");
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
}

void checkTemps(float humidity, float temperature)
{
  Serial.print("Evening - ");
  Serial.print(humidity);
  Serial.print("% ");
  Serial.print(temperature);
  Serial.println(F(" °F"));

  // Save data to SD card
  myFile = sd.open("data.txt", FILE_WRITE);
  if (myFile)
  {
    myFile.print("Evening Check - ");
    myFile.print("Humidity: ");
    myFile.print(humidity);
    myFile.print("%, ");
    myFile.print("Temperature: ");
    myFile.println(temperature);
    myFile.close();
  }
  else
  {
    Serial.println(F("Error opening file for writing"));
  }
}
