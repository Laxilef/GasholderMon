# GasholderMon
Monitoring the parameters of your gasholder.

![logo](/assets/logo.jpg)

## Features
- Monitoring of the remaining gas in percent and liters using the SRG level gauge ([with hall sensor](https://www.aliexpress.com/item/1005003713504199.html))
- Detection refueling: saving the number of liters and date
- Setting the service interval and a reminder of the need for service
- Possibility to use a moisture sensor (WL connector on pcb)
- [Home Assistant](https://www.home-assistant.io/) integration via MQTT: gas consumption history, low gas alerts, service reminder and more!

## Dependencies
- [ESP8266Scheduler](https://github.com/nrwiersma/ESP8266Scheduler)
- [NTPClient](https://github.com/arduino-libraries/NTPClient)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [TelnetStream](https://github.com/jandrassy/TelnetStream)
- [EEManager](https://github.com/GyverLibs/EEManager)
- [GyverFilters](https://github.com/GyverLibs/GyverFilters)
- [MultiMap](https://github.com/RobTillaart/MultiMap)
- [ADS1X15](https://github.com/RobTillaart/ADS1X15)
- [WiFiManager](https://github.com/tzapu/WiFiManager)

## PCB
![pcb sheet](/assets/pcb.svg)
![pcb 3d](/assets/pcb_3d.png)
- [Sheet](/assets/sheet.pdf)
- [BOM](/assets/BOM.xlsx)
- [Gerber](/assets/gerber.zip)

## Sensors connection
- The level gauge sensor must be connected to **RS IN** (signal) and **RS +** (power), GND to **GND**. On my sensor: black - gnd, red - power, brown - signal.
- Moisture sensor must be connected to **WL IN** (signal) and **WL +** (power).

## Settings
1. Connect to GasholderMon hotspot, password: gmon123456
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
  {81, 180, 1714, 3166, 4125, 4888, 6032, 7687, 8548, 9448, 10305, 11334, 12568, 13841, 15468, 17300, 18559, 19620, 20590, 21601, 22798, 23933},
  {0,  5,   7.5,  10,   12.5, 15,   20,   30,   35,   40,   45,    50,    55,    60,    65,    70,    75,    80,    85,    90,    95,    96}
};
```
The first element of the array is an array of sensor values, the second element of the array is an array of values in percent.