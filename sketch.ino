#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT22
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// BMP180
Adafruit_BMP085 bmp;

// RGB LED
#define RED    25
#define GREEN  26
#define BLUE   27

float temp, hum;
float pressure, altitude;
String weather;

void setup() {

  Serial.begin(115200);

  Wire.begin(21, 22);

  dht.begin();

  if (!bmp.begin()) {
    Serial.println("BMP180 NOT FOUND");
    while (1);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED FAILED");
    while (1);
  }

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  // Welcome Screen
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(12,20);
  display.println("WEATHER");
  display.display();

  delay(2000);
}

void loop() {

  // Read Sensors
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  pressure = bmp.readPressure() / 100.0;
  altitude = bmp.readAltitude();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT ERROR");
    return;
  }

  // Weather Prediction
  if (pressure > 1015) {

    weather = "Sunny";

    // Green LED
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);

  }
  else if (pressure >= 1005) {

    weather = "Cloudy";

    // Yellow LED (Red + Green)
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);

  }
  else {

    weather = "Rain";

    // Red LED
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);

  }

  // OLED Display
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Header
  display.setCursor(20,0);
  display.println("IoT WEATHER");

  display.drawLine(0,10,127,10,SSD1306_WHITE);

  // Temperature
  display.setCursor(0,14);
  display.print("Temp : ");
  display.print(temp,1);
  display.write(248);
  display.println("C");

  // Humidity
  display.setCursor(0,24);
  display.print("Hum  : ");
  display.print(hum,0);
  display.println("%");

  // Pressure
  display.setCursor(0,34);
  display.print("Pres : ");
  display.print(pressure,1);
  display.println(" hPa");

  // Altitude
  display.setCursor(0,44);
  display.print("Alt  : ");
  display.print(altitude,1);
  display.println(" m");

  display.drawLine(0,54,127,54,SSD1306_WHITE);

  // Weather
  display.setCursor(0,56);
  display.print(weather);

  // Temperature Status
  display.setCursor(70,56);

  if(temp > 35)
    display.print("HOT!");
  else
    display.print("NORMAL");

  display.display();

  // Serial Monitor
  Serial.print("Temperature : ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Humidity    : ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Pressure    : ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Altitude    : ");
  Serial.print(altitude);
  Serial.println(" m");

  Serial.print("Weather     : ");
  Serial.println(weather);

  Serial.println("---------------------------");

  delay(1000);
}