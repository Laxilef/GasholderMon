# GasholderMon
Monitoring the parameters of your gasholder.

![logo](/assets/logo.jpg)

## Features
- Monitoring of the remaining gas in percent and liters using the SRG level gauge ([with hall sensor](https://www.aliexpress.com/item/1005003713504199.html))
- Detection refueling: saving the number of liters and date
- Setting the service interval and a reminder of the need for service
- [Home Assistant](https://www.home-assistant.io/) integration via MQTT: gas consumption history, low gas alerts, service reminder and more!

## Dependencies
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [TelnetStream](https://github.com/jandrassy/TelnetStream)
- [EEManager](https://github.com/GyverLibs/EEManager)
- [ESP8266Scheduler](https://github.com/nrwiersma/ESP8266Scheduler)
- [NTPClient](https://github.com/arduino-libraries/NTPClient)
- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [GyverFilters](https://github.com/GyverLibs/GyverFilters)
- [MultiMap](https://github.com/RobTillaart/MultiMap)
- [WiFiManager](https://github.com/tzapu/WiFiManager)

## Sensor connection
The sensor must be connected to pin **A0**, and the power supply must be connected to **3v**. On my sensor: black - gnd, red - power, brown - signal (to A0).

**If the esp8266 is powered by 5v, then DO NOT connect the sensor to 5v!**

## Settings
1. Connect to GasholderMon hotspot
2. Open configuration page in browser: 192.168.4.1
3. Set up a connection to your wifi network
4. Set up a connection to your MQTT server

After connecting to your wifi network, you can go to the setup page at the address that esp8266 received.
The gasholdermon device will be automatically added to homeassistant if MQTT server ip, login and password are correct.

## HomeAsssistant settings
1. In the device settings, set the **capacity** of your gasholder in liters. Usually, the capacity is written in the passport of the gasholder.
2. If the remaining gas percentage is incorrect (less or more), try changing the **Calibration** parameter up or down in increments of 1.
3. You can also set the service interval. After the service is completed, click on the **Service button** to update the date.
4. Create automations to notify low gas and service needs.

## Debug & sensor conversion map
To display DEBUG messages you must enable debug in settings (switch is disabled by default).
You can connect via Telnet to read messages. IP: esp8266 ip, port: 23

The default conversion map is (Settings.h):
```
float rsCalibrationMap[2][22] = {
  {4, 9, 68,  135, 176,  210, 259, 329, 367, 412, 454, 496, 552, 609, 680, 765, 816, 874, 916, 955, 1011, 1020},
  {0, 5, 7.5, 10,  12.5, 15,  20,  30,  35,  40,  45,  50,  55,  60,  65,  70,  75,  80,  85,  90,  95,   96}
};
```
The first element of the array is an array of sensor values, the second element of the array is an array of values in percent.
