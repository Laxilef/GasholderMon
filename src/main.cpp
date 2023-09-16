#include <Arduino.h>
#include "defines.h"
#include <ArduinoJson.h>
#include <TelnetStream.h>
#include <EEManager.h>
#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Settings.h"

EEManager eeSettings(settings, 30000);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#include "WifiManagerTask.h"
#include "MqttTask.h"
#include "SensorsTask.h"
#include "MainTask.h"

// Tasks
WifiManagerTask* tWm;
MqttTask* tMqtt;
SensorsTask* tSensors;
MainTask* tMain;

void setup() {
#ifdef USE_TELNET
  TelnetStream.begin();
  delay(5000);
#else
  Serial.begin(115200);
  Serial.println("\n\n");
#endif

  EEPROM.begin(eeSettings.blockSize());
  uint8_t eeSettingsResult = eeSettings.begin(0, 'g');
  if (eeSettingsResult == 0) {
    INFO("Settings loaded");

  } else if (eeSettingsResult == 1) {
    INFO("Settings NOT loaded, first start");

  } else if (eeSettingsResult == 2) {
    INFO("Settings NOT loaded (error)");
  }

  tWm = new WifiManagerTask(true);
  Scheduler.start(tWm);

  tMqtt = new MqttTask(false);
  Scheduler.start(tMqtt);

  tSensors = new SensorsTask(true, SENSORS_INTERVAL);
  Scheduler.start(tSensors);

  tMain = new MainTask(true);
  Scheduler.start(tMain);

  timeClient.begin();
  Scheduler.begin();
}

void loop() {}