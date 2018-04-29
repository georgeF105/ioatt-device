#include "device-status.h"

#include <Arduino.h>

#include <ESP8266HTTPClient.h>
#include <storage.h>
#include <WebSocketsClient.h>

#define DEVICE_STATUS_ENDPOINT "/deviceStatus"

DeviceStatus::DeviceStatus (int updateRate, Storage *storage) {
  _updateRate = updateRate;
  _lastUpdated = millis();
  _storage = storage;
  _deviceKey = _storage->getDeviceKey();

  _webSocket.begin(REMOTE_SERVER_ADDRESS, 4081, "/");
  Serial.println("DeviceStatus::DeviceStatus");
  _webSocket.onEvent([this](WStype_t type, uint8_t * payload, size_t length) {
    _webSocketEvent(type, payload, length);
  });
  _webSocket.setReconnectInterval(5000);
}

boolean DeviceStatus::update () {
  _webSocket.loop();
  // if (_lastUpdated + _updateRate < millis()) {
  //   _lastUpdated = millis();
  //   fetchStatus();
  //   return true;
  // }
  return false;
}

void DeviceStatus::fetchStatus () {
  HTTPClient http;
  String url = String("http://") + String(REMOTE_SERVER_ADDRESS) + String(":3000") + String(DEVICE_STATUS_ENDPOINT);
  Serial.println(url);
  http.begin(url);
  http.addHeader("deviceKey", _deviceKey);

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
    Serial.print("failed to parse payload: ");
    Serial.println(_payload);
  }

  int value = payloadJson[valueName];

  return value;
}

boolean DeviceStatus::getBoolean (char *valueName) {
  StaticJsonBuffer<JSON_SIZE> jsonBuffer;
  JsonObject& payloadJson = jsonBuffer.parseObject(_payload);

  if (!payloadJson.success()) {
    Serial.print("failed to payload: ");
    Serial.println(_payload);
  }

  int value = payloadJson[valueName];

  return value;
}

void DeviceStatus::_webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

	switch(type) {
		case WStype_DISCONNECTED: {
			Serial.printf("[WSc] Disconnected!\n");
      _payload = "";
    }
			break;
		case WStype_CONNECTED: {
			Serial.printf("[WSc] Connected to url: %s\n", payload);

			// send message to server when Connected
			_webSocket.sendTXT(_deviceKey);
		}
			break;
		case WStype_TEXT:
			Serial.printf("[WSc] get text: %s\n", payload);

      _payload = String((char *)payload);

			// send message to server
			// _webSocket.sendTXT("message here");
			break;
		case WStype_BIN:
			Serial.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);

      _payload = String(*payload);
			// send data to server
			// _webSocket.sendBIN(payload, length);
			break;
	}
}
