extern PubSubClient client;

class HomeAssistantHelper {
public:
  void setPrefix(String value) {
    _prefix = value;
  }

  void setDeviceVersion(String value) {
    _deviceVersion = value;
  }

  void setDeviceManufacturer(String value) {
    _deviceManufacturer = value;
  }

  void setDeviceModel(String value) {
    _deviceModel = value;
  }

  void setDeviceName(String value) {
    _deviceName = value;
  }

  void setDeviceConfigUrl(String value) {
    _deviceConfigUrl = value;
  }


  bool publishSwitchDebug(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_debug";
    doc["object_id"] = _prefix + "_debug";
    doc["entity_category"] = "config";
    doc["name"] = "Debug";
    doc["state_topic"] = _prefix + F("/settings");
    doc["state_on"] = true;
    doc["state_off"] = false;
    doc["value_template"] = "{{ value_json.debug }}";
    doc["command_topic"] = _prefix + "/settings/set";
    doc["payload_on"] = "{\"debug\": true}";
    doc["payload_off"] = "{\"debug\": false}";

    client.beginPublish((F("homeassistant/switch/") + _prefix + "/debug/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }


  bool publishNumberCapacityLiters(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_capacity_liters";
    doc["object_id"] = _prefix + "_capacity_liters";
    doc["entity_category"] = "config";
    doc["unit_of_measurement"] = "liters";
    doc["name"] = "Capacity";
    doc["icon"] = "mdi:storage-tank";
    doc["state_topic"] = _prefix + F("/settings");
    doc["value_template"] = "{{ value_json.capacity.liters|int(0) }}";
    doc["command_topic"] = _prefix + "/settings/set";
    doc["command_template"] = "{\"capacity\": {\"liters\" : {{ value }}}}";
    doc["min"] = 1;
    doc["max"] = 65535;
    doc["step"] = 1;

    client.beginPublish((F("homeassistant/number/") + _prefix + "/capacity_liters/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishNumberCapacityCalibration(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_capacity_calibration";
    doc["object_id"] = _prefix + "_capacity_calibration";
    doc["entity_category"] = "config";
    doc["name"] = "Capacity calibration";
    doc["icon"] = "mdi:altimeter";
    doc["state_topic"] = _prefix + F("/settings");
    doc["value_template"] = "{{ value_json.capacity.calibration|int(0) }}";
    doc["command_topic"] = _prefix + "/settings/set";
    doc["command_template"] = "{\"capacity\": {\"calibration\" : {{ value }}}}";
    doc["min"] = -32768;
    doc["max"] = 32767;
    doc["step"] = 1;

    client.beginPublish((F("homeassistant/number/") + _prefix + "/capacity_calibration/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }


  bool publishSensorRemainingLiters(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["availability"]["topic"] = _prefix + F("/state");
    doc["availability"]["value_template"] = F("{{ iif(value_json.remaining.ready, 'online', 'offline') }}");
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_remaining_liters";
    doc["object_id"] = _prefix + "_remaining_liters";
    doc["entity_category"] = "diagnostic";
    doc["device_class"] = "volume_storage";
    doc["state_class"] = "measurement";
    doc["unit_of_measurement"] = "L";
    doc["name"] = "Remaining";
    doc["icon"] = "mdi:gauge";
    doc["state_topic"] = _prefix + F("/state");
    doc["value_template"] = "{{ value_json.remaining.liters|float(0)|round(2) }}";

    client.beginPublish((F("homeassistant/sensor/") + _prefix + "/remaining_liters/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishSensorRemainingPercentage(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["availability"]["topic"] = _prefix + F("/state");
    doc["availability"]["value_template"] = F("{{ iif(value_json.remaining.ready, 'online', 'offline') }}");
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_remaining_percentage";
    doc["object_id"] = _prefix + "_remaining_percentage";
    doc["entity_category"] = "diagnostic";
    //doc["device_class"] = "volume_storage";
    doc["state_class"] = "measurement";
    doc["unit_of_measurement"] = "%";
    doc["name"] = "Remaining";
    doc["icon"] = "mdi:gauge";
    doc["state_topic"] = _prefix + F("/state");
    doc["value_template"] = "{{ value_json.remaining.percentage|float(0)|round(2) }}";

    client.beginPublish((F("homeassistant/sensor/") + _prefix + "/remaining_percentage/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }


  bool publishBinSensorRefuelingProcess(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["availability"]["topic"] = _prefix + F("/state");
    doc["availability"]["value_template"] = F("{{ iif(value_json.remaining.ready, 'online', 'offline') }}");
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_refueling_process";
    doc["object_id"] = _prefix + "_refueling_process";
    doc["entity_category"] = "diagnostic";
    //doc["device_class"] = "connectivity";
    doc["name"] = "Refueling";
    doc["icon"] = "mdi:tanker-truck";
    doc["state_topic"] = _prefix + F("/state");
    doc["value_template"] = F("{{ iif(value_json.refueling.process, 'ON', 'OFF') }}");

    client.beginPublish((F("homeassistant/binary_sensor/") + _prefix + "/refueling_process/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishSensorRefuelingLiters(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["availability"]["topic"] = _prefix + F("/settings");
    doc["availability"]["value_template"] = F("{{ iif(value_json.refueling.date > 0, 'online', 'offline') }}");
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_refueling_liters";
    doc["object_id"] = _prefix + "_refueling_liters";
    doc["entity_category"] = "diagnostic";
    //doc["device_class"] = "gas";
    doc["state_class"] = "measurement";
    doc["unit_of_measurement"] = "liters";
    doc["name"] = "Refueling liters";
    doc["icon"] = "mdi:tanker-truck";
    doc["state_topic"] = _prefix + F("/settings");
    doc["value_template"] = "{{ value_json.refueling.liters|int(0) }}";

    client.beginPublish((F("homeassistant/sensor/") + _prefix + "/refueling_liters/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishSensorRefuelingDate(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["availability"]["topic"] = _prefix + F("/settings");
    doc["availability"]["value_template"] = F("{{ iif(value_json.refueling.date > 0, 'online', 'offline') }}");
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_refueling_date";
    doc["object_id"] = _prefix + "_refueling_date";
    doc["entity_category"] = "diagnostic";
    doc["device_class"] = "date";
    //doc["state_class"] = "measurement";
    doc["name"] = "Refueling date";
    doc["state_topic"] = _prefix + F("/settings");
    doc["value_template"] = "{{ value_json.refueling.date|timestamp_utc }}";

    client.beginPublish((F("homeassistant/sensor/") + _prefix + "/refueling_date/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }


  bool publishNumberCounterM3ratio(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_counter_m3ratio";
    doc["object_id"] = _prefix + "_counter_m3ratio";
    doc["entity_category"] = "config";
    doc["name"] = "Counter m3 ratio";
    doc["icon"] = "mdi:altimeter";
    doc["state_topic"] = _prefix + F("/settings");
    doc["value_template"] = "{{ value_json.counter.m3ratio|float(3) }}";
    doc["command_topic"] = _prefix + "/settings/set";
    doc["command_template"] = "{\"counter\": {\"m3ratio\" : {{ value }}}}";
    doc["min"] = 0.01;
    doc["max"] = 1;
    doc["step"] = 0.001;

    client.beginPublish((F("homeassistant/number/") + _prefix + "/counter_m3ratio/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishSensorCounterLiters(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_counter_liters";
    doc["object_id"] = _prefix + "_counter_liters";
    doc["entity_category"] = "diagnostic";
    doc["device_class"] = "gas";
    doc["state_class"] = "total_increasing";
    doc["unit_of_measurement"] = "liters";
    doc["name"] = "Counter";
    doc["icon"] = "mdi:counter";
    doc["state_topic"] = _prefix + F("/state");
    doc["value_template"] = "{{ value_json.counter.liters|float(0)|round(2) }}";

    client.beginPublish((F("homeassistant/sensor/") + _prefix + "/counter_liters/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishSensorCounterM3(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_counter_m3";
    doc["object_id"] = _prefix + "_counter_m3";
    doc["entity_category"] = "diagnostic";
    doc["device_class"] = "gas";
    doc["state_class"] = "total_increasing";
    doc["unit_of_measurement"] = "m³";
    doc["name"] = "Counter";
    doc["icon"] = "mdi:counter";
    doc["state_topic"] = _prefix + F("/state");
    doc["value_template"] = "{{ value_json.counter.m3|float(0)|round(3) }}";

    client.beginPublish((F("homeassistant/sensor/") + _prefix + "/counter_m3/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }


  bool publishButtonServiceRun(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_service_run";
    doc["object_id"] = _prefix + "_service_run";
    doc["entity_category"] = "config";
    doc["device_class"] = "restart";
    doc["name"] = "Service";
    doc["command_topic"] = _prefix + "/settings/set";
    doc["command_template"] = "{\"service\": {\"date\" : {{ utcnow() | as_timestamp | round(0) }}}}";

    client.beginPublish((F("homeassistant/button/") + _prefix + "/service_run/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishNumberServiceInterval(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_service_interval";
    doc["object_id"] = _prefix + "_service_interval";
    doc["entity_category"] = "config";
    doc["unit_of_measurement"] = "days";
    doc["name"] = "Service interval";
    doc["icon"] = "mdi:wrench-clock";
    doc["state_topic"] = _prefix + F("/settings");
    doc["value_template"] = "{{ value_json.service.interval|int(0) }}";
    doc["command_topic"] = _prefix + "/settings/set";
    doc["command_template"] = "{\"service\": {\"interval\" : {{ value }}}}";
    doc["min"] = 1;
    doc["max"] = 732;
    doc["step"] = 1;

    client.beginPublish((F("homeassistant/number/") + _prefix + "/service_interval/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishSensorServiceDate(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["availability"]["topic"] = _prefix + F("/settings");
    doc["availability"]["value_template"] = F("{{ iif(value_json.service.date > 0, 'online', 'offline') }}");
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_service_date";
    doc["object_id"] = _prefix + "_service_date";
    doc["entity_category"] = "diagnostic";
    doc["device_class"] = "date";
    //doc["state_class"] = "measurement";
    doc["name"] = "Service date";
    doc["icon"] = "mdi:wrench-clock";
    doc["state_topic"] = _prefix + F("/settings");
    doc["value_template"] = "{{ value_json.service.date|timestamp_utc }}";

    client.beginPublish((F("homeassistant/sensor/") + _prefix + "/service_date/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishBinSensorServiceRequired(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["availability"]["topic"] = _prefix + F("/settings");
    doc["availability"]["value_template"] = F("{{ iif(value_json.service.date > 0, 'online', 'offline') }}");
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_service_required";
    doc["object_id"] = _prefix + "_service_required";
    doc["entity_category"] = "diagnostic";
    doc["device_class"] = "problem";
    doc["name"] = "Service required";
    doc["icon"] = "mdi:wrench";
    doc["state_topic"] = _prefix + F("/state");
    doc["value_template"] = "{{ iif(value_json.service.required, 'ON', 'OFF') }}";

    client.beginPublish((F("homeassistant/binary_sensor/") + _prefix + "/service_required/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishBinSensorMoisture(bool enabledByDefault = true) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_moisture";
    doc["object_id"] = _prefix + "_moisture";
    doc["entity_category"] = "diagnostic";
    doc["device_class"] = "moisture";
    doc["name"] = "Moisture";
    doc["state_topic"] = _prefix + F("/state");
    doc["value_template"] = "{{ iif(value_json.service.moisture, 'ON', 'OFF') }}";

    client.beginPublish((F("homeassistant/binary_sensor/") + _prefix + "/moisture/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

  bool publishSensorRssi(bool enabledByDefault = false) {
    StaticJsonDocument<1536> doc;
    doc["device"]["identifiers"][0] = _prefix;
    doc["device"]["sw_version"] = _deviceVersion;
    doc["device"]["manufacturer"] = _deviceManufacturer;
    doc["device"]["model"] = _deviceModel;
    doc["device"]["name"] = _deviceName;
    if (_deviceConfigUrl) {
      doc["device"]["configuration_url"] = _deviceConfigUrl;
    }

    doc["enabled_by_default"] = enabledByDefault;
    doc["unique_id"] = _prefix + "_rssi";
    doc["object_id"] = _prefix + "_rssi";
    doc["entity_category"] = "diagnostic";
    doc["device_class"] = "signal_strength";
    doc["state_class"] = "measurement";
    doc["unit_of_measurement"] = "dBm";
    doc["name"] = "RSSI";
    doc["icon"] = "mdi:signal";
    doc["state_topic"] = _prefix + F("/state");
    doc["value_template"] = "{{ value_json.service.rssi|float(0)|round(1) }}";

    client.beginPublish((F("homeassistant/sensor/") + _prefix + "/rssi/config").c_str(), measureJson(doc), true);
    //BufferingPrint bufferedClient(client, 32);
    //serializeJson(doc, bufferedClient);
    //bufferedClient.flush();
    serializeJson(doc, client);
    return client.endPublish();
  }

private:
  String _prefix = "gasholdermon";
  String _deviceVersion = "1.0";
  String _deviceManufacturer = "Community";
  String _deviceModel = "GasholderMon";
  String _deviceName = "GasholderMon";
  String _deviceConfigUrl = "";
};
