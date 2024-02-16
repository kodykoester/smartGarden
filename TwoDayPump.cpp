#include <DHT.h>
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

  // Check if two days have passed to run pumps
  if (currentMillis - previousMorningMillis >= morningInterval && daysCounter < 7 && !stopProgram)
  {
    runPumps();
    previousMorningMillis = currentMillis; // Save the current time
    daysCounter++; // Increment the days counter
  }

  // Check if 8 hours have passed for continuous temperature and humidity checks
  if (currentMillis - previousEveningMillis >= eveningInterval && !stopProgram)
  {
    float h = dht.readHumidity();          // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    float f = dht.readTemperature(true);   // Read temperature as Fahrenheit (isFahrenheit = true)
    checkTemps(h, f);
    previousEveningMillis = currentMillis; // Save the current time
  }

  // Add a delay to reduce loop frequency
  delay(1000);
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
