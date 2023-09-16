#define GASHOLDERMON_VERSION    "1.1.1"
#define AP_SSID                 "GasholderMon"
#define AP_PASSWORD             "gmon123456"
#define USE_TELNET
#define SENSORS_INTERVAL        5000

#define MQTT_RECONNECT_INTERVAL 5000
#define MQTT_KEEPALIVE          30

#define ADS1115_ADDRESS         0x48
#define RS_MAP_POWER_VOLTAGE    5000
#define RS_DRIFT_DOWN_PERCENT   0.15
#define RS_DRIFT_UP_PERCENT     1.0
#define RS_POWER_PIN            D7
#define RS_POLLING_LED_PIN      D5
//#define REMAINING_FILTER_BUFFER 20

#define WL_POWER_PIN            D6
#define WL_STATUS_LED_PIN       D0
#define WL_UP_TRESHOLD          2.75
#define WL_DOWN_TRESHOLD        2.5

#define REFUELING_NUM_SIGNS     5
#define REFUELING_IDLE_INTERVAL 600000

#define CONFIG_URL              "http://%s/"


#ifdef USE_TELNET
  #define INFO_STREAM TelnetStream
  #define WARN_STREAM TelnetStream
  #define ERROR_STREAM TelnetStream
  #define DEBUG_STREAM  if (settings.debug) TelnetStream
  #define WM_DEBUG_PORT TelnetStream
#else
  #define INFO_STREAM Serial
  #define WARN_STREAM Serial
  #define ERROR_STREAM Serial
  #define DEBUG_STREAM  if (settings.debug) Serial
  #define WM_DEBUG_PORT Serial
#endif

#define INFO(...) INFO_STREAM.print("\r[INFO] "); INFO_STREAM.println(__VA_ARGS__);
#define INFO_F(...) INFO_STREAM.print("\r[INFO] "); INFO_STREAM.printf(__VA_ARGS__);
#define WARN(...) WARN_STREAM.print("\r[WARN] "); WARN_STREAM.println(__VA_ARGS__);
#define WARN_F(...) WARN_STREAM.print("\r[WARN] "); WARN_STREAM.printf(__VA_ARGS__);
#define ERROR(...) ERROR_STREAM.print("\r[ERROR] "); ERROR_STREAM.println(__VA_ARGS__);
#define DEBUG(...) DEBUG_STREAM.print("\r[DEBUG] "); DEBUG_STREAM.println(__VA_ARGS__);
#define DEBUG_F(...) DEBUG_STREAM.print("\r[DEBUG] "); DEBUG_STREAM.printf(__VA_ARGS__);