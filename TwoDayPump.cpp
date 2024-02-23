#include <DHT.h>
#include <SdFat.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define SD_FAT_TYPE 1
#define SD_CS_PIN 23
SdFat sd;
FsFile myFile;

const int pumpRelayOne = 7;
const int pumpRelayTwo = 13;

unsigned long previousMorningMillis = 0;
unsigned long previousEveningMillis = 0;

const long morningInterval = 172800000; // 2 days in milliseconds
const long eveningInterval = 28800000;  // 8 hours in milliseconds

int daysCounter = 0;
bool stopProgram = false;

void setup() {
  Serial.begin(115200);

  pinMode(pumpRelayOne, OUTPUT);
  pinMode(pumpRelayTwo, OUTPUT);
  digitalWrite(pumpRelayOne, LOW);
  digitalWrite(pumpRelayTwo, LOW);
  dht.begin();

  if (!sd.begin(SD_CS_PIN)) {
    Serial.println(F("SD initialization failed!"));
    exit(EXIT_FAILURE);
  }

  float initialHumidity = dht.readHumidity();
  float initialTemperature = dht.readTemperature(true);
  logData("Initial Check", initialHumidity, initialTemperature);
}

void loop() {
  unsigned long currentMillis = millis();
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  // Check if 2 days have passed
  if (currentMillis - previousMorningMillis >= morningInterval) {
    previousMorningMillis = currentMillis;
    runPumps();
    daysCounter += 2;

    // Check if 8 days have passed
    if (daysCounter >= 8) {
      Serial.println("Program will now end and shut down gracefully.");
      exit(EXIT_SUCCESS);
    }
  }

  // Check if 8 hours have passed for the evening check
  if (currentMillis - previousEveningMillis >= eveningInterval) {
    previousEveningMillis = currentMillis;
    checkTemps(h, f);
  }
}

void runPumps() {
  digitalWrite(pumpRelayOne, HIGH);
  digitalWrite(pumpRelayTwo, HIGH);

  float h = dht.readHumidity();
  float temp = dht.readTemperature(true);

  myFile = sd.open("data.txt", FILE_WRITE);
  if (myFile) {
    myFile.print("Pump Run: ");
    myFile.print(h);
    myFile.print("%, ");
    myFile.print("Temp: ");
    myFile.println(temp);
    myFile.close();
  } else {
    Serial.println(F("Error opening file for writing"));
  }

  delay(75000);

  digitalWrite(pumpRelayOne, LOW);
  digitalWrite(pumpRelayTwo, LOW);

  previousEveningMillis = millis();
}

void checkTemps(float humidity, float temperature) {
  myFile = sd.open("data.txt", FILE_WRITE);
  if (myFile) {
    myFile.print("2pm: ");
    myFile.print(humidity);
    myFile.print("%, ");
    myFile.print("Temp: ");
    myFile.println(temperature);
    myFile.println();

    float secondHumidity = dht.readHumidity();
    float secondTemperature = dht.readTemperature(true);
    
    myFile.print("10pm: ");
    myFile.print(secondHumidity);
    myFile.print("%, ");
    myFile.print("Temp: ");
    myFile.println(secondTemperature);
    
    myFile.close();
  } else {
    Serial.println(F("Error opening file for writing"));
  }
}

void logData(const char *timestamp, float humidity, float temperature) {
  myFile = sd.open("data.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(timestamp);
    myFile.print(": ");
    myFile.print(humidity);
    myFile.print("%, ");
    myFile.print("Temp: ");
    myFile.println(temperature);
    myFile.close();
  } else {
    Serial.println(F("Error opening file for writing"));
  }
}
