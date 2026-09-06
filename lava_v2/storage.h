#pragma once

#include <EEPROM.h>

class Storage {
  private:
    bool isInitialized = false;
    uint8_t launchNumber = 0;
    
    bool isInit() {
      if (!isInitialized) {
        Serial.println("EEPROM is not initialized");
        return false;
      }
      return true;
    }
  
  public:
    // Initializes the EEPROM, if needed
    void init() {
      Serial.println("Initializing EEPROM...");
      if (EEPROM.read(INIT_ADDR) != INIT_VAL) {
        EEPROM.write(INIT_ADDR, INIT_VAL);
        EEPROM.write(LAUNCH_NUM_ADDR, 0);
      }

      isInitialized = true;
      Serial.println("EEPROM initialized");
    }
    // Loads the data from the EEPROM to
    // the object
    bool load() {
      Serial.println("Loading data from EEPROM");
      if(isInit()) {
        EEPROM.get(LAUNCH_NUM_ADDR, launchNumber);
        Serial.println("Data loaded from EEPROM");
        return true;
      }
      Serial.println("Cannot load data if EEPROM is not initialized");
      return false;
    }

    // Stores data into the EEPROM
    bool store() {
      Serial.println("Storing data to EEPROM");
      if (isInit()) {
        EEPROM.update(LAUNCH_NUM_ADDR, launchNumber);
        Serial.println("Data stored to EEPROM");
        return true;
      }
      Serial.println("Cannot store data if EEPROM is not initialized");
      return false;
    }

    uint8_t getLaunchNumber() {
      return launchNumber; 
    }

    uint8_t incrementLaunchNumber() {
      return launchNumber++;
    }
};