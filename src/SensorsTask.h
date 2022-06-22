#include "GyverFilters.h"
#include "MultiMap.h"

class SensorsTask : public CustomTask {
public:
  SensorsTask(bool enabled = false, unsigned long interval = 0) : CustomTask(enabled, interval) {}

protected:
  GMedian<REMAINING_FILTER_BUFFER, unsigned short> remainingSensorBuffer;
  unsigned char rsMeasurements = 0;
  double prevRemainingPercentage = 0;
  unsigned char refuelingNumSigns = 0;
  unsigned long refuelingLastChange = 0;
  double beforeRefuelingPercentage = 0;

  void setup() {}

  void loop() {
    remaining();
    refueling();
    service();
  }

  void remaining() {
    unsigned short sensorValue = analogRead(REMAINING_SENSOR_PIN);
    unsigned short calibratedValue = sensorValue;

    if (settings.capacity.calibration < 0 && sensorValue < settings.capacity.calibration) {
      calibratedValue = 0;
    } else if (settings.capacity.calibration > 0 && sensorValue + settings.capacity.calibration > 1024) {
      calibratedValue = 1024;
    } else {
      calibratedValue += settings.capacity.calibration;
    }

    unsigned short filteredValue = remainingSensorBuffer.filtered(calibratedValue);

    if (!vars.remaining.ready) {
      if (++rsMeasurements < REMAINING_FILTER_BUFFER) {
        DEBUG_F("Remaining sensor value: %u, calibrated: %u, filtered: %u\n", sensorValue, calibratedValue, filteredValue);
        return;

      } else {
        vars.remaining.ready = true;
      }
    }

    double percentage = multiMap<float>(
      filteredValue,
      rsCalibrationMap[0],
      rsCalibrationMap[1],
      sizeof(rsCalibrationMap[0]) / sizeof(float)
    );
    prevRemainingPercentage = vars.remaining.percentage;
    vars.remaining.percentage = percentage;
    vars.remaining.liters = percentageToLiters(percentage);

    DEBUG_F(
      "Remaining sensor value: %u, calibrated: %u, filtered: %u, percentage: %f, liters: %f\n",
      sensorValue,
      calibratedValue,
      filteredValue,
      vars.remaining.percentage,
      vars.remaining.liters
    );
  }

  void refueling() {
    if (!vars.refueling.process && vars.remaining.percentage > prevRemainingPercentage) {
      if (refuelingNumSigns + 1 >= REFUELING_NUM_SIGNS) {
        vars.refueling.process = true;
        DEBUG("Refueling started...");

      } else {
        if (refuelingNumSigns == 0) {
          beforeRefuelingPercentage = prevRemainingPercentage;
        }

        refuelingLastChange = millis();
        refuelingNumSigns++;
      }

    } else if (!vars.refueling.process && refuelingNumSigns > 0 && millis() - refuelingLastChange > REFUELING_IDLE_INTERVAL / 10) {
      refuelingNumSigns = 0;
      refuelingLastChange = 0;
      beforeRefuelingPercentage = 0;

    } else if (vars.refueling.process && vars.remaining.percentage > prevRemainingPercentage) {
      refuelingLastChange = millis();

    } else if (vars.refueling.process && millis() - refuelingLastChange > REFUELING_IDLE_INTERVAL) {
      settings.refueling.date = timeClient.getEpochTime();
      settings.refueling.liters = round(percentageToLiters(vars.remaining.percentage - beforeRefuelingPercentage));

      vars.refueling.process = false;
      refuelingNumSigns = 0;
      refuelingLastChange = 0;
      beforeRefuelingPercentage = 0;

      DEBUG_F("Refueling completed: %u l.\n", settings.refueling.liters);
    }
  }

  void service() {
    if (!timeClient.getEpochTime()) {
      return;
    }

    if (settings.service.date > 0 && timeClient.getEpochTime() - settings.service.date > (settings.service.interval * 86400)) {
      if (!vars.service.required) {
        vars.service.required = true;
      }
    } else {
      if (vars.service.required) {
        vars.service.required = false;
      }
    }
  }

  double percentageToLiters(double percentage) {
    return round(percentage * 0.01 * settings.capacity.liters * 100) * 0.01;
  }
};