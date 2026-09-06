#pragma once

#include <SPI.h>
#include <SD.h>
#include "config.h"

class SDLogger {
  private:
    bool isInitialized = false;
    char* logFileName;

    bool isInit() {
      if(!isInitialized) {
        Serial.println("SD Card is not initialized");
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
        Serial.println("Could not initialize SD card");
        return false;
      }

      File logFile = SD.open(fileName, FILE_WRITE);
      
      if (!logFile) {
        Serial.println("Could not open file:");
        Serial.println(fileName);
        return false;
      }

      logFile.close();

      isInitialized = true;
      strncpy(logFileName, fileName, sizeof(fileName));

      return true;
    }


    // Writes a single line of data to the log file
    bool writeLineToSD(const String data) {
      if (!isInit()) return false;

      File logFile = SD.open(logFileName, FILE_WRITE);
      
      if (!logFile) {
        Serial.println("Failed to write to file");
        Serial.println(logFileName);
        return false;
      }

      size_t len = data.length(); // Add two for newline and carriage return
      size_t written = logFile.print(data);
      logFile.println();

      if (len != written) {
        Serial.println("Incomplete write to SD");
        logFile.close();
        return false;
      }

      logFile.flush();
      logFile.close();

      return true;
    }
};