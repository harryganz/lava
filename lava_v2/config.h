// SPI pins for the SD card
#define PIN_SD_MISO 12
#define PIN_SD_MOSI 11
#define PIN_SD_SCK  13
#define PIN_SD_CS   10

// -- I2C Bus ---
#define PIN_SDA    A4 // PIN 18
#define PIN_SCL    A5 // PIN 19

// --- Sample Rate ---
#define SAMPLE_RATE_HZ 50
#define SAMPLE_INTERVAL_MS (1000 / SAMPLE_RATE_HZ) // 20ms
#define ROLLING_WINDOW_SAMPLES 50 // ~1 s of history

// --- Flight detection thresholds ---
#define LAUNCH_ALTITUDE_RATE_MPS 10.0f  // m/s climb rate to detect launch
#define LANDING_ALTITUDE_RATE_MPS 2.0f  // m/s max rate to consider landed
#define LANDING_ALTITUDE_MARGIN_M 10.0f // must be within 10m of start (prevents landing detection at apogee)
#define INIT_ALT_AVG_DURATION_MS 3000   // 3s average to calculate initial altitude

// --- FRAM circular buffer ---
#define FRAM_I2C_ADDR   0x50
#define FRAM_SIZE_BYTES 4096
#define FRAM_FLUSH_THRESHOLD 0.75f // flush to SD at 75% full

// On-board EEPROM addresses
#define EEPROM_ADDR_STATE         0 // 1 byte, uint8_t, flight state
#define EEPROM_ADDR_FLIGHT_NUMBER 1 // 2 bytes, uint16_t, current flight number
#define EEPROM_ADDR_WRITE_PTR     3 // 2 bytes, uint16_t, offset in FRAM
#define EEPROM_ADDR_READ_PTR      5 // 2 bytes, uint16_t, offset into FRAM
#define EEPROM_ADDR_BYTES_WRITTEN 7 // 2 bytes, uint16_t
#define EEPROM_ADDR_LAST_MILLIS   9  // 4 bytes, uint32_t, last read time from sensors
#define EEPROM_ADDR_INITIAL_ALT   13 // 4 bytes, float

// --- BMP58X Settings ---
#define BMP58X_ODR       BMP5_ODR_50_HZ // Sampling target rate
#define BMP58X_IIR_COEFF BMP5_IIR_FILTER_COEFF_3 // Number of samples to feed to IIR filer
#define BMP580_OSR_PRESS    BMP5_OVERSAMPLING_4X // Pressure oversampling
#define BMP580_OSR_TEMP     BMP5_OVERSAMPLING_1X // Temperature oversampling

// --- LSM6DS Settings ---
#define LSM6DS_ACCEL_RANGE  LSM6DS_ACCEL_RANGE_16_G
#define LSM6DS_GYRO_RANGE   LSM6DS_GYRO_RANGE_2000_DPS
#define LSM6DS_ODR          LSM6DS_RATE_104_HZ         

// --- Watchdog ---
#define WATCHDOG_TIMEOUT    WDTO_500MS
