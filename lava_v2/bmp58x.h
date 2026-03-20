#include <Adafruit_BMP5xx.h>

class Altimeter {
public:
    // Configure BMP580 with settings from config.h (ODR, IIR, OSR).
    // Returns false if sensor not found on I2C bus.
    bool begin();

    // Read the current pressure altitude in metres. Returns NAN on failure.
    float readAltitude();

    // Read raw temperature in Celsius. Returns NAN on failure.
    float readTemperature();

    // Block for INIT_ALT_AVG_DURATION_MS, sampling at SAMPLE_RATE_HZ,
    // and return the mean altitude. Used during init to set baseline.
    // Also performs a stillness check: returns NAN if stddev > 0.5m,
    // indicating the rocket is not stable enough to calibrate.
    float calibrateBaselineAltitude();

    // True if the last read succeeded.
    bool isHealthy() const;

private:
    Adafruit_BMP5xx _bmp;
    bool _healthy = false;
};