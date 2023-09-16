//#include <GyverFilters.h>
#include <MultiMap.h>
#include <ADS1X15.h>

class SensorsTask : public Task {
public:
  SensorsTask(bool _enabled = false, unsigned long _interval = 0) : Task(_enabled, _interval) {}

protected:
  ADS1115* ads;
  //RingAverage<unsigned short, REMAINING_FILTER_BUFFER> remainingSensorBuffer;
  //unsigned char rsMeasurements = 0;
  double prevRemainingPercentage = 0;
  unsigned char refuelingNumSigns = 0;
  unsigned long refuelingLastChange = 0;
  double beforeRefuelingPercentage = 0;
  double counterPrevRemaining = 0;


  void setup() {
    pinMode(RS_POLLING_LED_PIN, OUTPUT);
    pinMode(RS_POWER_PIN, OUTPUT);

    pinMode(WL_STATUS_LED_PIN, OUTPUT);
    pinMode(WL_POWER_PIN, OUTPUT);

    digitalWrite(RS_POLLING_LED_PIN, false);
    digitalWrite(RS_POWER_PIN, true);

    digitalWrite(WL_STATUS_LED_PIN, false);
    digitalWrite(WL_POWER_PIN, true);


    ads = new ADS1115(ADS1115_ADDRESS);
    ads->begin();
    ads->setGain(0);
    ads->setDataRate(0);
    ads->setMode(1);

    vars.counter.liters = settings.counter.liters;
    vars.counter.m3 = litersToM3(vars.counter.liters);
  }

  void loop() {
    static unsigned long lastServiceCheck = 0;

    remaining();
    refueling();
    counter();
    
    if (millis() - lastServiceCheck > 30000) {
      yield();
      service();
      moisture();
      lastServiceCheck = millis();
    }
  }

  void remaining() {
    if ( !ads->isConnected() ) {
      WARN(F("[SENSORS][REMAINING] ADS1115 not connected"));
      return;
    }
    
    digitalWrite(RS_POLLING_LED_PIN, true);
    digitalWrite(RS_POWER_PIN, false);
    delay(150);

    // power value
    DEBUG(F("[SENSORS][REMAINING] Request power voltage..."));
    ads->requestADC(0);
    while ( !ads->isReady() ) {
      DEBUG(F("[SENSORS][REMAINING] Response not ready, waiting..."));
      delay(50);
    }
    short powerValue = ads->getValue();
    double compensation = double(RS_MAP_POWER_VOLTAGE / 1000 * 32767.0 / 6.144) / double(powerValue);
    DEBUG_F("[SENSORS][REMAINING] Power value: %d, voltage: %f, compensation: %f\n", powerValue, ads->toVoltage(powerValue), compensation);

    // sensor value
    DEBUG(F("[SENSORS][REMAINING] Request sensor value..."));
    ads->requestADC_Differential_0_1();
    while ( !ads->isReady() ) {
      DEBUG(F("[SENSORS][REMAINING] Response not ready, waiting..."));
      delay(50);
    }

    digitalWrite(RS_POLLING_LED_PIN, false);
    digitalWrite(RS_POWER_PIN, true);

    short response = ads->getValue();
    unsigned int sensorValue = ( response < powerValue ) ? powerValue - response : 0;
    unsigned int compensatedValue, calibratedValue;
    if ( settings.capacity.calibration > 1 or settings.capacity.calibration < 1 ) {
      compensatedValue = round(double(sensorValue) * compensation);
      calibratedValue = round(double(sensorValue) * compensation * settings.capacity.calibration);
    } else {
      compensatedValue = round(double(sensorValue) * compensation);
      calibratedValue = compensatedValue;
    }
    DEBUG_F(
      "[SENSORS][REMAINING] Sensor value: %d (%f v), compensated: %d (%f v), calibrated: %d (%f v)\n",
      sensorValue,
      ads->toVoltage(sensorValue),
      compensatedValue,
      ads->toVoltage(compensatedValue),
      calibratedValue,
      ads->toVoltage(calibratedValue)
    );

    /*unsigned short filteredValue = round(remainingSensorBuffer.filteredFloat(calibratedValue));
    if (!vars.remaining.ready) {
      if (++rsMeasurements < REMAINING_FILTER_BUFFER) {
        DEBUG_F("Remaining sensor value: %u, calibrated: %u, filtered: %u\n", sensorValue, calibratedValue, filteredValue);
        return;

      } else {
        vars.remaining.ready = true;
      }
    }*/

    double percentage = multiMap<float>(
      //filteredValue,
      calibratedValue,
      rsCalibrationMap[2],
      rsCalibrationMap[3],
      sizeof(rsCalibrationMap[2]) / sizeof(float)
    );

    if ( !vars.remaining.ready || (percentage - vars.remaining.percentage + 0.0001) >= RS_DRIFT_UP_PERCENT || (vars.remaining.percentage - percentage + 0.0001) >= RS_DRIFT_DOWN_PERCENT ) {
      prevRemainingPercentage = vars.remaining.percentage;
      vars.remaining.percentage = percentage;
      vars.remaining.liters = percentageToLiters(percentage);

      if ( !vars.remaining.ready ) {
        vars.remaining.ready = true;
      }
    }

    DEBUG_F(
      "[SENSORS][REMAINING] Percentage no filtering: %f (%f l), percentage with filtering: %f (%f l)\n",
      percentage,
      percentageToLiters(percentage),
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

  void counter() {
    static bool flagAfterReady = true;
    static bool flagAfterRefueling = false;

    if (!vars.remaining.ready) {
      flagAfterReady = true;
      return;
    }

    if (vars.refueling.process) {
      flagAfterRefueling = true;
      return;
    }

    if ( (flagAfterReady || flagAfterRefueling) && vars.remaining.liters > 0 ) {
      counterPrevRemaining = vars.remaining.liters;

      if ( flagAfterReady ) {
        flagAfterReady = false;
      }

      if ( flagAfterRefueling ) {
        flagAfterRefueling = false;
      }
    }

    double consumption = counterPrevRemaining - vars.remaining.liters;
    if ( consumption >= 0.1 ) {
      counterPrevRemaining = vars.remaining.liters;
      vars.counter.liters += consumption;
      vars.counter.m3 = litersToM3(vars.counter.liters);
    }

    if ( vars.counter.liters - settings.counter.liters + 0.0001 >= 1 ) {
      settings.counter.liters = vars.counter.liters;
      eeSettings.update();
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

  void moisture() {
    if ( !ads->isConnected() ) {
      WARN(F("[SENSORS][MOISTURE] ADS1115 not connected"));
      return;
    }
    
    digitalWrite(WL_POWER_PIN, false);
    delay(150);

    // sensor value
    DEBUG(F("[SENSORS][MOISTURE] Request sensor value..."));
    ads->requestADC_Differential_0_3();
    while ( !ads->isReady() ) {
      DEBUG(F("[SENSORS][MOISTURE] Response not ready, waiting..."));
      delay(50);
    }

    digitalWrite(WL_POWER_PIN, true);

    short response = ads->getValue();
    float voltage = ads->toVoltage(response);
    DEBUG_F("[SENSORS][MOISTURE] Sensor value: %d (%f v)\n", response, voltage);

    if ( voltage >= WL_UP_TRESHOLD ) {
      vars.service.moisture = true;
      digitalWrite(WL_STATUS_LED_PIN, true);

    } else if ( voltage < WL_DOWN_TRESHOLD ) {
      vars.service.moisture = false;
      digitalWrite(WL_STATUS_LED_PIN, false);
    }
  }

  double percentageToLiters(double percentage) {
    return round(percentage * 0.01 * settings.capacity.liters * 100) * 0.01;
  }

  double litersToM3(double liters) {
    return round(liters * settings.counter.m3ratio * 1000) * 0.001;
  }
};