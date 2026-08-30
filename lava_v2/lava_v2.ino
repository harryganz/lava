#include "config.h"
#include "storage.h"
#include "sd_logger.h"

void setup() {
  // Initialize Serial
  Serial.begin(9600);
  while(!Serial){}

  // Initialize EEPROM
  Storage storage = Storage();
  storage.init();
  if(!storage.load()) while(1);

  // Setup logger
  SDLogger logger = SDLogger();

  char logName[16];
  snprintf(logName, sizeof(logName), "launch-%d.txt", storage.getLaunchNumber());
  
  
  if(!logger.begin(logName)) while(1);
  if(!logger.writeLineToSD("hello world!")) while(1);
  storage.incrementLaunchNumber();
  if(!storage.store()) while(1);
}

void loop() {

}