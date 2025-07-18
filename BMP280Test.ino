#include "DHT.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define DHT_TYPE DHT22
const int DHT_PIN = 12;
DHT dht(DHT_PIN, DHT_TYPE);

float outsideTemperature;
float humidity;
float heatIndex;

/* Sensor for indoor temperature and air pressure */
Adafruit_BMP280 bmp;
#define SEALEVELPRESSURE_HPA (1013.25)

float indoorTemperature;
float pressure;
float altitude;

const long dataReadInterval = 5000;
unsigned long previousDataReadTime = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  bool status = bmp.begin(0x76);
  if (!status) {
    Serial.println("No BMP280 sensor found, check wiring!");
    while (1); // stop program
  }
}

void loop() {
  const long currentTime = millis();
  if (currentTime - previousDataReadTime >= dataReadInterval) {
    previousDataReadTime = currentTime;
    readSensorData();
    }
}

void readSensorData() {
  humidity = dht.readHumidity();
  outsideTemperature = dht.readTemperature();
  heatIndex = dht.computeHeatIndex(outsideTemperature, humidity, false);
  indoorTemperature = bmp.readTemperature();
  pressure = bmp.readPressure() / 100.0F;
  altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print("Outside temperature: "); Serial.print(outsideTemperature);
  Serial.print("°C  Humidity: "); Serial.print(humidity);
  Serial.print("%  Feels like: "); Serial.print(heatIndex);
  Serial.println("°C");
  Serial.print("Inside temperature: "); Serial.print(indoorTemperature);
  Serial.print("°C  Air pressure: "); Serial.print(pressure);
  Serial.print(" hPa  Altitude above sea level: "); Serial.print(altitude);
  Serial.println(" m");
}