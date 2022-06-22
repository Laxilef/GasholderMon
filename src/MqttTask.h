#include <WiFiClient.h>
#include <PubSubClient.h>
#include <netif/etharp.h>
#include "HomeAssistantHelper.h"

WiFiClient espClient;
PubSubClient client(espClient);
HomeAssistantHelper haHelper;


class MqttTask : public CustomTask {
public:
  MqttTask(bool enabled = false, unsigned long interval = 0) : CustomTask(enabled, interval) {}

protected:
  unsigned long lastReconnectAttempt = 0;

  void setup() {
    client.setServer(settings.mqtt.server, settings.mqtt.port);
    client.setCallback(__callback);
    haHelper.setPrefix(settings.mqtt.prefix);
    haHelper.setDeviceVersion(GASHOLDERMON_VERSION);
  }

  void loop() {
    if (!client.connected() && millis() - lastReconnectAttempt >= MQTT_RECONNECT_INTERVAL) {
      INFO_F("Mqtt not connected, state: %i, connecting to server %s...\n", client.state(), settings.mqtt.server);

      if (client.connect(settings.hostname, settings.mqtt.user, settings.mqtt.password)) {
        INFO("Connected to MQTT server");

        client.subscribe(getTopicPath("settings/set").c_str());
        client.subscribe(getTopicPath("state/set").c_str());
        publishHaEntities();

        lastReconnectAttempt = 0;

      } else {
        INFO("Failed to connect to MQTT server\n");

        forceARP();
        lastReconnectAttempt = millis();
      }
    }


    if (client.connected()) {
      client.loop();
      publish();
    }
  }


  static void forceARP() {
    struct netif* netif = netif_list;
    while (netif) {
      etharp_gratuitous(netif);
      netif = netif->next;
    }
  }

  static bool updateSettings(JsonDocument& doc) {
    bool flag = false;

    if (!doc["debug"].isNull() && doc["debug"].is<bool>()) {
      settings.debug = doc["debug"].as<bool>();
      flag = true;
    }

    if (!doc["mqtt"]["interval"].isNull() && doc["mqtt"]["interval"].is<unsigned int>() && doc["mqtt"]["interval"].as<unsigned int>() >= 1000 && doc["mqtt"]["interval"].as<unsigned int>() <= 120000) {
      settings.mqtt.interval = doc["mqtt"]["interval"].as<unsigned int>();
      flag = true;
    }

    // capacity
    if (!doc["capacity"]["liters"].isNull() && doc["capacity"]["liters"].is<unsigned short>() && doc["capacity"]["liters"].as<unsigned short>() > 0 && doc["capacity"]["liters"].as<unsigned short>() <= 65535) {
      settings.capacity.liters = doc["capacity"]["liters"].as<unsigned short>();
      flag = true;
    }

    if (!doc["capacity"]["calibration"].isNull() && doc["capacity"]["calibration"].is<short>() && doc["capacity"]["calibration"].as<short>() >= -32768 && doc["capacity"]["calibration"].as<short>() <= 32767) {
      settings.capacity.calibration = doc["capacity"]["calibration"].as<short>();
      flag = true;
    }

    // refueling
    if (!doc["refueling"]["liters"].isNull() && doc["refueling"]["liters"].is<unsigned short>() && doc["refueling"]["liters"].as<unsigned short>() >= 0 && doc["refueling"]["liters"].as<unsigned short>() <= 65535) {
      settings.refueling.liters = doc["refueling"]["liters"].as<unsigned short>();
      flag = true;
    }

    if (!doc["refueling"]["date"].isNull() && doc["refueling"]["date"].is<unsigned long>() && doc["refueling"]["date"].as<unsigned long>() >= 0) {
      settings.refueling.date = doc["refueling"]["date"].as<unsigned long>();
      flag = true;
    }

    // service
    if (!doc["service"]["interval"].isNull() && doc["service"]["interval"].is<unsigned short>() && doc["service"]["interval"].as<unsigned short>() >= 0 && doc["service"]["interval"].as<unsigned short>() <= 732) {
      settings.service.interval = doc["service"]["interval"].as<unsigned short>();
      flag = true;
    }

    if (!doc["service"]["date"].isNull() && doc["service"]["date"].is<unsigned long>() && doc["service"]["date"].as<unsigned long>() >= 0) {
      settings.service.date = doc["service"]["date"].as<unsigned long>();
      flag = true;
    }

    if (!doc["restart"].isNull() && doc["restart"].is<bool>() && doc["restart"].as<bool>()) {
      eeSettings.updateNow();
      publish(true);
      ESP.restart();

      return true;
    }

    if (flag) {
      eeSettings.update();
      publish(true);

      return true;
    }

    return false;
  }

  static bool updateVariables(const JsonDocument& doc) {
    bool flag = false;

    if (!doc["ping"].isNull() && doc["ping"]) {
      flag = true;
    }

    if (flag) {
      publish(true);

      return true;
    }

    return false;
  }

  static void publish(bool force = false) {
    static unsigned int prevPubVars = 0;
    static unsigned int prevPubSettings = 0;

    // publish variables and status
    if (force || millis() - prevPubVars > settings.mqtt.interval) {
      publishVariables(getTopicPath("state").c_str());

      forceARP();
      prevPubVars = millis();
    }

    // publish settings
    if (force || millis() - prevPubSettings > settings.mqtt.interval * 10) {
      publishSettings(getTopicPath("settings").c_str());
      prevPubSettings = millis();
    }
  }

  static void publishHaEntities() {
    // main
    haHelper.publishSwitchDebug(false);

    // capacity
    haHelper.publishNumberCapacityLiters();
    haHelper.publishNumberCapacityCalibration();

    // remaining
    haHelper.publishSensorRemainingLiters();
    haHelper.publishSensorRemainingPercentage();

    // refueling
    haHelper.publishBinSensorRefuelingProcess();
    haHelper.publishSensorRefuelingLiters();
    haHelper.publishSensorRefuelingDate();

    // service
    haHelper.publishButtonServiceRun();
    haHelper.publishNumberServiceInterval();
    haHelper.publishSensorServiceDate();
    haHelper.publishBinSensorServiceRequired();
  }

  static bool publishSettings(const char* topic) {
    StaticJsonDocument<2048> doc;

    doc["debug"] = settings.debug;

    doc["capacity"]["liters"] = settings.capacity.liters;
    doc["capacity"]["calibration"] = settings.capacity.calibration;

    doc["refueling"]["liters"] = settings.refueling.liters;
    doc["refueling"]["date"] = settings.refueling.date;

    doc["service"]["interval"] = settings.service.interval;
    doc["service"]["date"] = settings.service.date;

    client.beginPublish(topic, measureJson(doc), false);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  static bool publishVariables(const char* topic) {
    StaticJsonDocument<2048> doc;

    doc["remaining"]["ready"] = vars.remaining.ready;
    doc["remaining"]["liters"] = vars.remaining.liters;
    doc["remaining"]["percentage"] = vars.remaining.percentage;

    doc["refueling"]["process"] = vars.refueling.process;
    doc["service"]["required"] = vars.service.required;

    client.beginPublish(topic, measureJson(doc), false);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  static std::string getTopicPath(const char* topic) {
    return std::string(settings.mqtt.prefix) + "/" + std::string(topic);
  }

  static void __callback(char* topic, byte* payload, unsigned int length) {
    if (!length) {
      return;
    }

    if (settings.debug) {
      DEBUG_F("MQTT received message\n\r        Topic: %s\n\r        Data: ", topic);
      for (int i = 0; i < length; i++) {
        DEBUG_STREAM.print((char)payload[i]);
      }
      DEBUG_STREAM.print("\n");
    }

    StaticJsonDocument<2048> doc;
    DeserializationError dErr = deserializeJson(doc, (const byte*)payload, length);
    if (dErr != DeserializationError::Ok || doc.isNull()) {
      return;
    }

    if (getTopicPath("state/set").compare(topic) == 0) {
      updateVariables(doc);
      client.publish(getTopicPath("state/set").c_str(), NULL, true);

    } else if (getTopicPath("settings/set").compare(topic) == 0) {
      updateSettings(doc);
      client.publish(getTopicPath("settings/set").c_str(), NULL, true);
    }
  }
};