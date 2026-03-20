#include "flight_record.h"
#include <Adafruit_FRAM_I2C.h>

class FramBuffer {
public:
    // Initialise the FRAM over I2C. Returns false if the chip is not found.
    bool begin();

    // Write one FlightRecord at the given byte offset.
    // Caller (main sketch) is responsible for pointer arithmetic and wrap-around.
    bool writeRecord(uint16_t byteOffset, const FlightRecord& record);

    // Read one FlightRecord from the given byte offset.
    bool readRecord(uint16_t byteOffset, FlightRecord& record);

    // Erase the entire FRAM (write zeros). Called when starting a new flight
    // after a successful flush to SD.
    void erase();

    // Returns the maximum number of records that fit in FRAM.
    uint16_t capacity() const;   // FRAM_MAX_RECORDS (113)

    // Returns true if the chip responds on I2C.
    bool isConnected();

private:
    Adafruit_FRAM_I2C _fram;
};