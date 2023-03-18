#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLLm9q5cfa"
#define BLYNK_DEVICE_NAME "weather mointering systtem"

#include <WiFi.h> // importing all the required libraries
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Arduino.h"
#include "DHT.h"
#include "BMP085.h"
#include <Wire.h>

float temperature; // parameters
float humidity;
float pressure;
float altitude;
float mbar;
bool lightval;
float rainval;

BMP085 myBarometer; // initialise pressure sensor

char auth[] = "YR982_vzaWdSjJk8ZdCl2hvhH1N9pcva"; // replace this with your auth token
char ssid[] = "Private"; // replace this with your wifi name (SSID)
char pass[] = "123456789"; // replace this with your wifi password

#define DHTPIN 5 // dht sensor is connected to D5
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

const int LDR_PIN = 36;
const int RAIN_PIN = 39;

DHT dht(DHTPIN, DHTTYPE); // initialise dht sensor
BlynkTimer timer;

void sendSensor() // function to read sensor values and send them to Blynk
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP()); // read pressure value in pascals
  mbar = pressure / 100; // convert millibar to pascals
  altitude = myBarometer.calcAltitude();

  lightval= digitalRead(LDR_PIN);
 

  rainval= analogRead(RAIN_PIN);
  rainval = map(rainval, 0, 1023, 100, 0);

  Blynk.virtualWrite(V0, temperature); // send all the values to their respective virtual pins
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, mbar);
  Blynk.virtualWrite(V3, altitude);
  Blynk.virtualWrite(V4, lightval);
  Blynk.virtualWrite(V5, rainval);
 
}

void setup()
{
  Serial.begin(115200);
 
  myBarometer.init();
  dht.begin();
  delay(1000);
  Blynk.begin(auth, ssid, pass);
  delay(1000);
  timer.setInterval(10L, sendSensor); // sendSensor function will run every 1000 milliseconds
}

void loop()
{
  Blynk.run();
  timer.run();
}
