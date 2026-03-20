#include <SD.h>

class SDLogger {
public:
    // Initialise SPI and SD library. Returns false on failure.
    bool begin();

    // Open a new binary file: "FLT_NNN.BIN" where NNN = zero-padded flight number.
    // Returns false if file already exists or SD error.
    bool openNewFlight(uint16_t flightNumber);

    // Open an existing flight file for appending (used during recovery flush).
    // Returns false if file does not exist.
    bool openExistingFlight(uint16_t flightNumber);

    // Write a single FlightRecord to the currently open file.
    // Does NOT flush to disk — call flush() explicitly to commit.
    bool writeRecord(const FlightRecord& record);

    // Write a block of N records from a byte array. Used for bulk FRAM flush.
    bool writeBlock(const uint8_t* data, uint16_t numBytes);

    // Flush the file write buffer to the SD card. Slow (~10-80ms).
    // Call when safe to stall (e.g. on landing, or during partial flush on pad).
    bool flush();

    // Close the currently open file. Always call before power down.
    void close();

    // True if a file is currently open and healthy.
    bool isOpen() const;

    // Re-initialise SD after an error (for recovery path).
    bool reinit();

private:
    File   _file;
    bool   _sdReady = false;
    bool   _fileOpen = false;
};