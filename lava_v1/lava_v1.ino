#include <SPI.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

Adafruit_BMP280 bmp;  // I2C

// Variables
unsigned long lastAltReadTime = 0.0;
unsigned long lastDisplayTime = 0.0;

float referencePressure = 1013.25;  // Pa
long readAltInterval = 50;          // ms
long displayAltInterval = 1000;     // ms

float initialAlt = 0.0;
;
float currentAlt = 0.0;
float maxAlt = 0.0;

void setup() {
  Serial.begin(9600);

  // Wait for display
  delay(100);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1)
      ;
  }

  // Initialize BMP280
  if (!bmp.begin()) {
    Serial.println(F("Failed to initialize BMP280"));
    while (1)
      ;
  }

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);  // Use full 256 char font

  display.setCursor(0, 8);
  display.print(F("Waiting for device to be ready ..."));
  display.display();

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,  // Collects data intermittently
                  Adafruit_BMP280::SAMPLING_X1,  // Temp. oversampling
                  Adafruit_BMP280::SAMPLING_X8,  // Pressure oversampling
                  Adafruit_BMP280::FILTER_X8,    // Iterations used for filtering
                  Adafruit_BMP280::STANDBY_MS_1  // Microseconds of standby
  );

  // Initialize variables
  delay(5000);  // Wait 5 seconds to read initial altitude
  initialAlt = bmp.readAltitude(referencePressure);
  maxAlt = initialAlt;
  lastAltReadTime = millis();
  lastDisplayTime = lastAltReadTime;
}

void loop() {
  unsigned long currentTime = millis();
  // Check altitude every read alt interval
  if (currentTime - lastAltReadTime >= readAltInterval) {
    currentAlt = bmp.readAltitude(referencePressure) - initialAlt;
    if (currentAlt > maxAlt) {
      maxAlt = currentAlt;
    }
    lastAltReadTime += readAltInterval;
  }
  // Display max altitude once per displayAltInterval
  if (currentTime - lastDisplayTime >= displayAltInterval) {
    displayAltitude(maxAlt);
    lastDisplayTime += displayAltInterval;
  }
}

void displayAltitude(float alt) {
  display.clearDisplay();
  display.setCursor(0, 8);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);  // Use full 256 char font
  display.print(String(long(round(alt))) + "m");
  display.display();
}