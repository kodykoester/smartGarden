#include <DHT.h>
#include <TimeLib.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

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
  setTime(12, 0, 0, 2, 4, 2024); // Set initial date and time (replace these values with your desired date and time)

  pinMode(pumpRelayOne, OUTPUT);
  pinMode(pumpRelayTwo, OUTPUT);
  digitalWrite(pumpRelayOne, LOW);
  digitalWrite(pumpRelayTwo, LOW);
  dht.begin();             // Sensor Start
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
      stopProgram = true;  // Set the variable to exit the program gracefully
    }
  }

  // Check if it's time for the evening check
  if (currentMillis - previousEveningMillis >= eveningInterval)
  {
    previousEveningMillis = currentMillis; // Save the current time for the evening check
    checkTemps(h, f);                      // Run the checkTemps function in the evening
  }

  // Graceful exit
  if (stopProgram)
  {
    Serial.println("Program will now stop gracefully.");
    while (true)
    {
      // You can add any cleanup or other tasks here if needed
    }
  }
}

void runPumps()
{
  digitalWrite(pumpRelayOne, HIGH);
  digitalWrite(pumpRelayTwo, HIGH);
  Serial.println("Pumps running!");
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
}

void printTimeAndDate() {
  // Print hr, min, sec, month, date
  int hr = hour();
  int min = minute();
  int monthVal = month();
  int dateVal = day();

  // Convert 24-hour time to 12-hour time with AM/PM
  int ampm = 0; // 0 for AM, 1 for PM
  if (hr >= 12) {
    hr = hr - 12;
    ampm = 1;
  }
  if (hr == 0) {
    hr = 12;
  }

  // Print the formatted time and date
  Serial.print(hr);
  printDigits(min);
  Serial.print((ampm == 1) ? "pm" : "am");
  Serial.print(" ");
  Serial.print(monthVal);
  Serial.print("/");
  Serial.print(dateVal);
  Serial.println();
}



void printDigits(int digits) {
  // Helper function to print leading zeros
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
