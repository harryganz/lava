#include "config.h"
#include "sd_logger.h"

void setup() {
  Serial.begin(9600);
  while(!Serial){}
  SDLogger logger = SDLogger();
  if(logger.begin("test.txt")) {
    logger.writeLineToSD("hello world!");
  }
}

void loop() {

}