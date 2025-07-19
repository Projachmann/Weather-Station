#include "DHT.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

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

/* LCD display output */
LiquidCrystal_I2C lcd(0x27, 16, 2);
const long dataReadInterval = 5000;
unsigned long previousDataReadTime = 0;

// Variables for text output on the display
String line1Text;
String line2Text;
String line3Text;
String line4Text;

int displayPage = LOW; // Flag for switching between page 1+2 and 3+4 on LCD

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  bool status = bmp.begin(0x76);
  if (!status) {
    Serial.println("No BMP280 sensor found, check wiring!");
    while (1);
  }

  /* Initialize LCD display */
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("+Weather Station+");
  lcd.setCursor(2, 1);
  lcd.print("Welcome!");
}
                
void loop() {
  const long currentTime = millis();
  if (currentTime - previousDataReadTime >= dataReadInterval) {
    previousDataReadTime = currentTime;
    readSensorData();
        
    if (displayPage == LOW) {
      displayPage = HIGH;
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print(line1Text);
      lcd.setCursor(0, 1); lcd.print(line2Text);
    } else {
      displayPage = LOW;
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print(line3Text);
      lcd.setCursor(0, 1); lcd.print(line4Text);
    }
  }
}
                                                                        
void readSensorData() {
  humidity = dht.readHumidity();
  outsideTemperature = dht.readTemperature();
  heatIndex = dht.computeHeatIndex(outsideTemperature, humidity, false);
    
  indoorTemperature = bmp.readTemperature();
  pressure = bmp.readPressure() / 100.0F;
  altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
          
  // Serial monitor output
  Serial.print("Outside temperature: "); Serial.print(outsideTemperature);
  Serial.print("°C  Humidity: "); Serial.print(humidity);
  Serial.print("%  Feels like: "); Serial.print(heatIndex);
  Serial.println("°C");
      
  Serial.print("Indoor temperature: "); Serial.print(indoorTemperature);
  Serial.print("°C  Air pressure: "); Serial.print(pressure);
  Serial.print(" hPa  Altitude above sea level: "); Serial.print(altitude);
  Serial.println(" m");
    
  // LCD output
  line1Text = "OutTemp: " + String(outsideTemperature, 1) + "C";
  line2Text = "InTemp: " + String(indoorTemperature, 1) + "C";
  line3Text = "Pressure: " + String(pressure, 1) + "hPa";
  line4Text = "Humidity: " + String(humidity, 1) + "%";
}