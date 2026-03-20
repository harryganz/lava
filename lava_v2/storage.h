#include <EEPROM.h>

enum class FlightState : uint8_t {
    LANDED    = 0,
    ON_PAD    = 1,
    IN_FLIGHT = 2,
    RECOVERY  = 3,
    ERROR     = 4
};

class Storage {
public:
    // Read all persisted fields from AVR EEPROM into RAM.
    // Called once at startup before any decisions are made.
    void load() {
        
    }

    // Write all fields back to AVR EEPROM.
    // Called whenever any field changes.
    void save();

    // Getters / setters for individual fields.
    // Each setter calls save() immediately for crash safety.
    FlightState getState() const;
    void        setState(FlightState s);

    uint16_t    getFlightNumber() const;
    void        setFlightNumber(uint16_t n);

    uint16_t    getWritePtr() const;
    void        setWritePtr(uint16_t ptr);

    uint16_t    getReadPtr() const;
    void        setReadPtr(uint16_t ptr);

    uint16_t    getBytesWritten() const;
    void        setBytesWritten(uint16_t n);

    uint32_t    getLastMillis() const;
    void        setLastMillis(uint32_t ms);

    float       getInitialAltitude() const;
    void        setInitialAltitude(float alt);

    // Returns true if the stored values pass a basic sanity check
    // (e.g. pointers are within FRAM bounds, state is a valid enum value).
    // If not, resets all fields to safe defaults and saves.
    bool validate() {

    }
};