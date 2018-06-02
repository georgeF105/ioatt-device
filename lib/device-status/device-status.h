#ifndef DEVICE_STATUS_H
#define DEVICE_STATUS_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <storage.h>
#include <WebSocketsClient.h>

#define JSON_SIZE 200
#define PING_RATE 10000 

class DeviceStatus {
  public:
    DeviceStatus (Storage *storage);
    void fetchStatus ();
    void loop ();
    int getInt (char *valueName);
    boolean getBoolean (char *valueName);

  private:
    unsigned long _lastUpdated;
    int _pingFailCount;
    Storage *_storage;
    String _deviceKey;
    String _payload;
    int _httpCode;
    WebSocketsClient _webSocket;
    void _webSocketEvent (WStype_t type, uint8_t * payload, size_t length);
};
#endif