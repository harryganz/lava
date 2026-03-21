#pragma once
#include <EEPROM.h>
#include "config.h"

enum class FlightState : uint8_t {
    LANDED    = 0,
    ON_PAD    = 1,
    IN_FLIGHT = 2,
    RECOVERY  = 3,
    ERROR     = 4
};

class Storage {
private:
    FlightState flightState;
    uint16_t    flightNumber;
    uint16_t    writePtr;
    uint16_t    readPtr;
    uint16_t    bytesWritten;
    uint32_t    lastMillis;
    float       iniitalAltitude;

public:
    // Read all persisted fields from AVR EEPROM into RAM.
    // Called once at startup before any decisions are made.
    void load() {
        EEPROM.get(EEPROM_ADDR_STATE, this->flightState);
        EEPROM.get(EEPROM_ADDR_FLIGHT_NUMBER, this->flightNumber);
        EEPROM.get(EEPROM_ADDR_WRITE_PTR, this->writePtr);
        EEPROM.get(EEPROM_ADDR_READ_PTR, this->readPtr);
        EEPROM.get(EEPROM_ADDR_BYTES_WRITTEN, this->bytesWritten);
        EEPROM.get(EEPROM_ADDR_LAST_MILLIS, this->lastMillis);
        EEPROM.get(EEPROM_ADDR_INITIAL_ALTITUDE, this->iniitalAltitude);
    }

    // Write all fields back to AVR EEPROM.
    // Called whenever any field changes.
    void save() {
        EEPROM.put(EEPROM_ADDR_STATE, this->flightState);
        EEPROM.put(EEPROM_ADDR_FLIGHT_NUMBER, this->flightNumber);
        EEPROM.put(EEPROM_ADDR_WRITE_PTR, this->writePtr);
        EEPROM.put(EEPROM_ADDR_READ_PTR, this->readPtr);
        EEPROM.put(EEPROM_ADDR_BYTES_WRITTEN, this->bytesWritten);
        EEPROM.put(EEPROM_ADDR_LAST_MILLIS, this->lastMillis);
        EEPROM.put(EEPROM_ADDR_INITIAL_ALTITUDE, this->iniitalAltitude);
    }

    // Getters / setters for individual fields.
    // Each setter calls save() immediately for crash safety.
    FlightState getState() const {
        return this->flightState;
    }
    void setState(FlightState s) {
        this->flightState=s;
    }

    uint16_t    getFlightNumber() const {
        return this->flightNumber;
    }
    void        setFlightNumber(uint16_t n) {
        this->flightNumber=n;
    }

    uint16_t    getWritePtr() const {
        return this->writePtr;
    }
    void        setWritePtr(uint16_t ptr) {
        this->writePtr=ptr;
    }

    uint16_t    getReadPtr() const {
        return this->readPtr;
    }
    void        setReadPtr(uint16_t ptr) {
        this->readPtr=ptr
    }

    uint16_t    getBytesWritten() const {
        return this->bytesWritten;
    }
    void        setBytesWritten(uint16_t n) {
        this->bytesWritten=n;
    }

    uint32_t    getLastMillis() const {
        return this->lastMillis;
    }
    void        setLastMillis(uint32_t ms) {
        this->lastMillis=ms;
    }

    float       getInitialAltitude() const {
        return this->iniitalAltitude;
    }
    void        setInitialAltitude(float alt) {
        this->iniitalAltitude=alt;
    }

    // Returns true if the stored values pass a basic sanity check
    // (e.g. pointers are within FRAM bounds, state is a valid enum value).
    // If not, resets all fields to safe defaults and saves.
    bool validate() {
        return (
            this->flightState >= 0 && this->flightState <= 4 &&
            this->flightNumber >= 0 &&
            this->writePtr >= 0 && this->writePtr <= FRAM_SIZE_BYTES &&
            this->readPtr >= 0 && this->readPtr <= FRAM_SIZE_BYTES &&
            this->bytesWritten >= 0 &&
            this->lastMillis >= 0 &&
            this->iniitalAltitude >= 0.0

        );
    }

};