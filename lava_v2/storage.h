#pragma once

#include <EEPROM.h>

class Storage {
  private:
    bool isInitialized = false;
    uint8_t launchNumber = 0;
    float initialAltitude = -999.f;
    
    bool isInit() {
      if (!isInitialized) {
        Serial.println(F("EEPROM is not initialized"));
        return false;
      }
      return true;
    }
  
  public:
    // Initializes the EEPROM, if needed
    void init() {
      Serial.println(F("Initializing EEPROM..."));
      if (EEPROM.read(INIT_ADDR) != INIT_VAL) {
        EEPROM.write(INIT_ADDR, INIT_VAL);
      }

      isInitialized = true;
      Serial.println(F("EEPROM initialized"));
    }
    // Loads the data from the EEPROM to
    // the object
    bool load() {
      Serial.println(F("Loading data from EEPROM"));
      if(isInit()) {
        EEPROM.get(LAUNCH_NUM_ADDR, launchNumber);
        EEPROM.get(INIT_ALT_ADDR, initialAltitude);
        Serial.println(F("Data loaded from EEPROM"));
        return true;
      }
      Serial.println(F("Cannot load data if EEPROM is not initialized"));
      return false;
    }

    // Stores data into the EEPROM
    bool store() {
      Serial.println(F("Storing data to EEPROM"));
      if (isInit()) {
        EEPROM.update(LAUNCH_NUM_ADDR, launchNumber);
        EEPROM.update(INIT_ALT_ADDR, initialAltitude);
        Serial.println(F("Data stored to EEPROM"));
        return true;
      }
      Serial.println(F("Cannot store data if EEPROM is not initialized"));
      return false;
    }

    uint8_t getLaunchNumber() {
      return launchNumber; 
    }

    uint8_t incrementLaunchNumber() {
      return launchNumber++;
    }

    float getInitAlttidue() {
      return initialAltitude;
    }

    float setInitAltitude(float alt) {
      initialAltitude = alt;
    }
};