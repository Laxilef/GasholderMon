extern MqttTask* tMqtt;

class MainTask : public LeanTask {
public:
  MainTask(bool _enabled = false, unsigned long _interval = 0) : LeanTask(_enabled, _interval) {}

protected:
  void setup() {}

  void loop() {
    static unsigned long lastHeapInfo = 0;
    static unsigned short minFreeHeapSize = 65535;

    if (eeSettings.tick()) {
      INFO("Settings updated (EEPROM)");
    }

    if (WiFi.status() == WL_CONNECTED) {
      timeClient.update();

      vars.service.rssi = WiFi.RSSI();
    }

    if (WiFi.status() == WL_CONNECTED && !tMqtt->isEnabled()) {
      tMqtt->enable();

    } else if (WiFi.status() != WL_CONNECTED && tMqtt->isEnabled()) {
      tMqtt->disable();
    }

#ifdef USE_TELNET
    // anti memory leak
    TelnetStream.flush();
    while (TelnetStream.available() > 0) {
      TelnetStream.read();
    }
#endif

    if (settings.debug) {
      unsigned short freeHeapSize = ESP.getFreeHeap();
      unsigned short minFreeHeapSizeDiff = 0;

      if (freeHeapSize < minFreeHeapSize) {
        minFreeHeapSizeDiff = minFreeHeapSize - freeHeapSize;
        minFreeHeapSize = freeHeapSize;
      }
      if (millis() - lastHeapInfo > 60000 || minFreeHeapSizeDiff > 0) {
        DEBUG_F("Free heap size: %hu bytes, min: %hu bytes (diff: %hu bytes)\n", freeHeapSize, minFreeHeapSize, minFreeHeapSizeDiff);
        lastHeapInfo = millis();
      }
    }
  }
};