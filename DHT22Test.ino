#include "DHT.h"

#define DHT_TYPE DHT22
const int DHT_PIN = 12;
DHT dht(DHT_PIN, DHT_TYPE);

float outsideTemperature;
float humidity;
float heatIndex;

const long dataReadInterval = 5000;
unsigned long previousDataReadTime = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
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
            
  Serial.print("Temperature: ");
  Serial.print(outsideTemperature);
  Serial.print("°C Humidity: ");
  Serial.print(humidity);
  Serial.print("% Feels like: ");
  Serial.print(heatIndex);
  Serial.println("°C");
}