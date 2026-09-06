#include "config.h"
#include "storage.h"
#include "sd_logger.h"
#include "altimeter.h"

Storage storage = Storage();
SDLogger logger = SDLogger();
Altimeter altimeter = Altimeter();

void setup() {
  Serial.println(F("Initializing flight logger..."));

  // Initialize Serial
  Serial.begin(9600);
  while(!Serial){}
  // Initialize EEPROM
  storage.init();
  if(!storage.load()) while(1);
  storage.incrementLaunchNumber();

  // Setup logger
  char logName[16];
  snprintf(logName, sizeof(logName), "%d.txt", storage.getLaunchNumber());
  if(!logger.begin(logName)) while(1);

  // Set altimeter
  if(!altimeter.init()) while(1);
  storage.setInitAltitude(altimeter.getInititialAltitude());

  // Store initial values to EEPROM
  if(!storage.store()) while(1);

  delay(10);
  Serial.println(F("Finished initializing flight logger"));
}

unsigned long t = 0;
float altitude = -999;
char buffer[16];

void loop() {
  if ((millis() - t) > 1000) {
    altitude = altimeter.getAltitude();
    dtostrf(altitude, 0, 2, buffer);
    logger.writeLineToSD(buffer);
    t = millis();
  }
}