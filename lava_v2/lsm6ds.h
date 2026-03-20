#include <Adafruit_LSM6DS.h>

class IMU {
public:
    // Configure LSM6DSO with settings from config.h (accel range, gyro range, ODR).
    // Returns false if not found.
    bool begin();

    // Read the latest accelerometer sample (m/s²). Returns false on failure.
    bool readAccel(float& x, float& y, float& z);

    // Read the latest gyroscope sample (rad/s). Returns false on failure.
    bool readGyro(float& x, float& y, float& z);

    // Read both in one I2C transaction where possible. Preferred in the main loop.
    bool readAll(float& ax, float& ay, float& az,
                 float& gx, float& gy, float& gz);

    bool isHealthy() const;

private:
    Adafruit_LSM6DS _imu;
    bool _healthy = false;
};