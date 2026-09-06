// Chip select pin for SD card
#define SD_CS_PIN    10

// Addresses and sizes (in bytes) for stored variables in onboard EEPROM
#define INIT_ADDR 0
#define INIT_VAL 0xC5 // Random byte to tell that the EEPROM was evern initialized
#define LAUNCH_NUM_ADDR 1
#define LAUNCH_NUM_SIZE sizeof(uint8_t)
#define INIT_ALT_ADDR LAUNCH_NUM_ADDR + LAUNCH_NUM_SIZE
#define INIT_ALT_SIZE sizeof(float)



