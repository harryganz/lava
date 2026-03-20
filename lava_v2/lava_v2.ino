#include "config.h"
#include "flight_record.h"
#include "storage.h"
#include "fram.h"
#include "bmp58x.h"
#include "lsm6ds.h"
#include "sd_log.h"
#include <avr/wdt.h>

Storage storage;
FramBuffer framBuf;
Altimeter altimeter;
IMU imu;
SDLogger sdLogger;

// Computes altitude rate of change (m/s) using the last ROLLING_WINDOW_SAMPLES
// records from the circular buffer. Reads back from FRAM using the read pointer.
// Returns NAN if insufficient samples are available.
float computeAltitudeRate();

// Called every loop tick when ON_PAD.
// Transitions to IN_FLIGHT if altitude rate > LAUNCH_ALTITUDE_RATE_MPS.
// The ON_PAD circular write continues during this time, overwriting old pad data,
// ensuring ~2s of pre-launch data is captured at the moment of launch detection.
void checkForLaunch();

// Called every loop tick when IN_FLIGHT.
// Transitions to LANDED if altitude rate < LANDING_ALTITUDE_RATE_MPS
// AND current altitude is within LANDING_ALTITUDE_MARGIN_M of initial altitude.
// On transition: saves state, triggers flushFramToSD().
void checkForLanding();

// Read all sensors and return a populated FlightRecord.
FlightRecord sampleSensors(uint32_t timestampMs);

// Write one record to FRAM at writePtr, then advance writePtr (with wrap).
// Update bytesWritten (capped at FRAM_SIZE_BYTES).
// In ON_PAD state, also advance readPtr to maintain a fixed-size rolling window,
// effectively discarding the oldest record when full.
void writeRecordToBuffer(const FlightRecord& rec);

// Check if bytes written >= FRAM_FLUSH_THRESHOLD * FRAM_SIZE_BYTES.
// If so, flush the oldest half of the buffer to SD and advance readPtr.
// Does NOT flush the most recent ~50 records (keep rolling window intact).
void checkAndFlushIfNeeded();

// Read all unread records from readPtr to writePtr and write them to SD.
// Advances readPtr as it goes. Calls sdLogger.flush() at the end.
// Safe to call at any time but will block for up to ~100ms on large writes.
void flushFramToSD();

// Transition to ERROR state, log the reason string to SD if possible, save state.
void enterErrorState(const char* reason);

// Initialise all three sensor/storage peripherals (FRAM, altimeter, IMU).
// Returns false if any one fails.
bool initAllSensors();

void setup(void) {
  wdt_enable(WATCHDOG_TIMEOUT);
  storage.load();
  if (!storage.validate()) {
    // Initial startup or corrupt EEPROM, treat
    // as fresh boot
    storage.setState(FlightState::LANDED);
    storage.setFlightNumber(0);
    storage.setBytesWritten(0);
  }

  FlightState state = storage.getState();
  
  // Attempt recovery if not landed
  if (state != FlightState::LANDED) {
    runRecoveryProcedure();
    state = storage.getState();
  }

  if (state == FlightState::LANDED) {
    // Flush data from previous flight, if present
    if (storage.getBytesWritten() > 0) {
      flushFramToSD();
    }
    storage.setBytesWritten(0);
    storage.setWritePtr(0);
    storage.setReadPtr(0);

    uint16_t nextFlight = storage.getFlightNumber() + 1;
    storage.setFlightNumber(nextFlight);

    if (!initAllSensors()) {
      enterErrorState("Sensor init failed");
      return;
    }

    if (!sdLogger.openNewFlight(nextFlight)) {
      enterErrorState("SD open failed");
      return;
    }
    
    float baseline = altimeter.calibrateBaselineAltitude();
    if (isnan(baseline)) {
      enterErrorState("Altitude calibraion failed");
      return;
    }
    storage.setInitialAltitude(baseline);
    storage.setState(FlightState::ON_PAD);
    storage.save();
  }


}

uint32_t lastSampleMs = 0;

void loop(void) {
  wdt_reset();

  uint32_t now = millis();
  if (now - lastSampleMs < SAMPLE_INTERVAL_MS) return;
  lastSampleMs = now;

  FlightState state = storage.getState();
  if (state == FlightState::ON_PAD || state == FlightState::IN_FLIGHT) {
    FlightRecord rec = sampleSensors(now);
    writeRecordToBuffer(rec);
    storage.setLastMillis(now);
  }


  if (state == FlightState::IN_FLIGHT) {
    checkAndFlushIfNeeded();
  }

  if (state == FlightState::ON_PAD) {
    checkForLaunch();
  } else if (state == FlightState::IN_FLIGHT) {
    checkForLanding();
  } else if (state == FlightState::LANDED) {
    // FLight is over, so log final buffer and halt
    flushFramToSD();
    while (true) {
      wdt_reset();
    }
  }

  if (state == FlightState::ERROR) {
    flushFramToSD();
    while (true) {
      wdt_reset();
    }
  }
}

// Called when we boot into non-landed state, e.g. 
// e.g. power is lost mid-flight. If still, transitions to LANDED.
// if moving, or sensors fail, transitions to error
void runRecoveryProcedure() {
  storage.setState(FlightState::RECOVERY);
  storage.save();

  if (!initAllSensors()) {
    enterErrorState("Recovery: sensor init failed");
    return;
  }

  // Check altitude for 1 second to see if it is changing
  float altStart = altimeter.readAltitude();
  delay(1000);
  wdt_reset();
  float altEnd = altimeter.readAltitude();

  if (isnan(altStart) || isnan(altEnd)) {
    enterErrorState("Recovery altimeter read failed");
    return;
  }

  float delta = abs(altEnd - altStart);
  if (delta < LANDING_ALTITUDE_MARGIN_M) {
    storage.setState(FlightState::LANDED);
  } else {
    enterErrorState("Recover: rocket appears to be moving");
  }
  storage.save();
}