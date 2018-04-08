#include "device-status.h"

#include <Arduino.h>

#include <ESP8266HTTPClient.h>

#define DEVICE_STATUS_ENDPOINT "/deviceStatus"

DeviceStatus::DeviceStatus (int updateRate) {
  _updateRate = updateRate;
  _lastUpdated = millis();
}

boolean DeviceStatus::update () {
  if (_lastUpdated + _updateRate < millis()) {
    _lastUpdated = millis();
    fetchStatus();
    return true;
  }
  return false;
}

void DeviceStatus::fetchStatus () {
  HTTPClient http;
  String url = String("http://") + String(REMOTE_SERVER_ADDRESS) + String(":3000") + String(DEVICE_STATUS_ENDPOINT);
  Serial.println(url);
  http.begin(url);

  _httpCode = http.GET();

  if(_httpCode > 0) {

    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", _httpCode);

    // file found at server
    if(_httpCode == HTTP_CODE_OK) {
      _payload = http.getString();
      Serial.println(_payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(_httpCode).c_str());
  }

  http.end();
}

int DeviceStatus::getInt (char *valueName) {
  StaticJsonBuffer<JSON_SIZE> jsonBuffer;
  JsonObject& payloadJson = jsonBuffer.parseObject(_payload);

  if (!payloadJson.success()) {
    Serial.print("failed to parseObject");
  }

  int value = payloadJson[valueName];
  payloadJson.prettyPrintTo(Serial);

  return value;
}
