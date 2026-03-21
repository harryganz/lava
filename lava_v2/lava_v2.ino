#include "config.h"
#include "flight_record.h"
#include "storage.h"
#include "fram.h"
#include "bmp58x.h"
#include "lsm6ds.h"
#include "sd_log.h"
#include <avr/wdt.h>

Storage storage;

// Resets values in EEPROM to safe defaults
void resetStorageToDefaults() {
  // Initial state will be landed as that is how it will end after each flight
  storage.setState(FlightState.LANDED);
  storage.setFlightNumber(0);
  storage.setReadPtr(0);
  storage.setWritePtr(0);
  storage.setBytesWritten(0);
  storage.setLastMillis(0);
  storage.setInitialAltitude(0.0);
}

void setup() {
  // Read state data from EEPROM and validate
  storage.load();
  // If not valid (e.g. if initial load from empty ROM) set to safe defaults
  if (!storage.validate()) {
    resetStorageToDefaults();
  }
  storage.save();
}

void loop() {

}