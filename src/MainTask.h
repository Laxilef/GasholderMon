class MainTask : public CustomTask {
public:
  MainTask(bool enabled = false, unsigned long interval = 0) : CustomTask(enabled, interval) {}

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
    }

    if (WiFi.status() == WL_CONNECTED && !tMqtt->isEnabled()) {
      tMqtt->enable();

    } else if (WiFi.status() != WL_CONNECTED && tMqtt->isEnabled()) {
      tMqtt->disable();
    }

#ifdef USE_TELNET
    yield();

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
      if (millis() - lastHeapInfo > 10000 || minFreeHeapSizeDiff > 0) {
        DEBUG_F("Free heap size: %hu bytes, min: %hu bytes (diff: %hu bytes)\n", freeHeapSize, minFreeHeapSize, minFreeHeapSizeDiff);
        lastHeapInfo = millis();
      }
    }
  }

  /*char[] getUptime() {
    uint64_t =  esp_timer_get_time();
  }*/
};