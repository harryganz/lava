#pragma once

#include <SPI.h>
#include <SD.h>
#include "config.h"

class SDLogger {
  private:
    bool isInitialized = false;
    char logFileName[16];

    bool isInit() {
      if(!isInitialized) {
        Serial.println(F("SD Card is not initialized"));
        return false;
      }
      return true;
    }

  public:
    // Initializes SD card and checks/creates log file
    // returns false on failure
    bool begin(const char* fileName) {
      if (isInitialized) return true;

      if (!SD.begin(SD_CS_PIN)) {
        Serial.println(F("Could not initialize SD card"));
        return false;
      }

      File logFile = SD.open(fileName, FILE_WRITE);
      
      if (!logFile) {
        Serial.println(F("Could not open file:"));
        Serial.println(fileName);
        return false;
      }

      logFile.close();

      strncpy(logFileName, fileName, sizeof(logFileName) - 1);
      logFileName[sizeof(logFileName) - 1] = '\0'; 

      isInitialized = true;
      return true;
    }


    // Writes a single line of data to the log file
    bool writeLineToSD(const char* data) {
      if (!isInit()) return false;

      File logFile = SD.open(logFileName, FILE_WRITE);
      
      if (!logFile) {
        Serial.println(F("Failed to write to file"));
        Serial.println(logFileName);
        return false;
      }

      size_t len = strlen(data); // Add two for newline and carriage return
      size_t written = logFile.print(data);
      logFile.println();

      if (len != written) {
        Serial.println(F("Incomplete write to SD"));
        logFile.close();
        return false;
      }

      logFile.flush();
      logFile.close();

      return true;
    }
};