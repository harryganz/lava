#pragma once
#include <stdint.h>

struct __attribute__((packed)) FlightRecord {
  uint32_t timestamp_ms; // millis() since boot (or last recovery)
  float altitude_m;      // pressure altitude in meters
  float accel_x;         // m/s^2, sensor frame
  float accel_y;
  float accel_z;
  float gyro_x;          // rad/s, sensor frame
  float gyro_y;
  float gyro_z;
};

// 32 bytes per record
// 4096 bytes FRAM / 32 bytes = 128 records max, 2.56s at 50Hz
// Circular buffer required; flush at 75% (96 records)
static_assert(sizeof(FlightRecord) == 32, "FlightRecord size mismatch");

#define FLIGHT_RECORD_SIZE sizeof(FlightRecord)
#define FRAM_MAX_RECORDS (FRAM_SIZE_BYTES / FLIGHT_RECORD_SIZE)