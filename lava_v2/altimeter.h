#pragma once

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP5xx.h"


#define SEA_LEVEL_PRESSURE_HPA (1013.25)

class Altimeter {
  private:
    Adafruit_BMP5xx bmp;
    bool isInitialized = false;

    bool isInit() {
      if (!isInitialized) {
        Serial.println("Altimeter is not initialized");
        return false;
      }
      return true;
    }
  public:
    bool init() {
      if (isInitialized) return true;
      
      Serial.println("Initializing altimeter...");

      // Initialize SPI/BMP
      Wire.begin();
      if (!bmp.begin(BMP5XX_ALTERNATIVE_ADDRESS, &Wire)) {
        Serial.println("Could not find BMP5xx");
        return false;
      }

      // Settings for Altimeter
      // Oversampling rates
      bmp.setTemperatureOversampling(BMP5XX_OVERSAMPLING_2X);
      bmp.setPressureOversampling(BMP5XX_OVERSAMPLING_8X);
      // Noise reduction
      bmp.setIIRFilterCoeff(BMP5XX_IIR_FILTER_COEFF_3);
      // Output rate
      bmp.setOutputDataRate(BMP5XX_ODR_50_HZ);
      // Power mode. Normal means continuous
      bmp.setPowerMode(BMP5XX_POWERMODE_NORMAL);
      bmp.enablePressure(true);

      isInitialized = true;
      Serial.println("Altimieter is initialized");

      return true;
    }

    // returns the current altitude
    // if not initialized will return an invalid float value (-999)
    float getAltitude() {
      if (!isInit()) return -999;

      return bmp.readAltitude(SEA_LEVEL_PRESSURE_HPA);
    }

};