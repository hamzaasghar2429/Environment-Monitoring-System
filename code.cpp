#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Share Arduino reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor Pin Definitions
#define DHTPIN 2       // DHT data pin connected to D2
#define DHTTYPE DHT11  // Blue module is typically DHT11. Change to DHT22 if you are using the white one.
#define SMOKE_PIN A0   // MQ-2 analog pin connected to A0
#define RAIN_PIN A1    // Rain sensor analog pin connected to A1

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize OLED display (Address 0x3C is standard for 0.96" I2C OLEDs)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Clear the display buffer
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

void loop() {
  // 1. Read Temperature and Humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // 2. Read Smoke Sensor (Analog value 0-1023)
  int smokeValue = analogRead(SMOKE_PIN);
  
  // 3. Read Rain Sensor (Analog value 0-1023)
  // Lower values mean more water (less resistance)
  int rainValue = analogRead(RAIN_PIN);
  String rainStatus = "Clear";
  
  if (rainValue < 400) {
    rainStatus = "Heavy Rain";
  } else if (rainValue < 800) {
    rainStatus = "Light Rain";
  } else {
    rainStatus = "Clear";
  }

  // 4. Update OLED Display
  display.clearDisplay();
  display.setCursor(0, 0);

  // Print Temperature
  display.print("Temp: ");
  if (isnan(t)) {
    display.println("Error");
  } else {
    display.print(t);
    display.println(" C");
  }

  // Print Humidity
  display.print("Humidity: ");
  if (isnan(h)) {
    display.println("Error");
  } else {
    display.print(h);
    display.println(" %");
  }

  // Print Smoke/Gas
  display.print("Smoke: ");
  display.print(smokeValue);
  display.println(" PPM"); // Note: True PPM requires calibration; this displays raw analog mapping for simplicity.

  // Print Rain Status
  display.print("Rain: ");
  display.println(rainStatus);

  // Render buffer to screen
  display.display();

  // Wait 2 seconds between readings (DHT11 requires at least 2 seconds between polls)
  delay(2000);
}