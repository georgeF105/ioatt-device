#ifndef DEVICE_STATUS_H
#define DEVICE_STATUS_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <storage.h>

#define JSON_SIZE 200

class DeviceStatus {
  public:
    DeviceStatus (int updateRate, Storage *storage);
    void fetchStatus ();
    boolean update ();
    int getInt (char *valueName);
    boolean getBoolean (char *valueName);

  private:
    int _updateRate;
    int _lastUpdated;
    Storage *_storage;
    String _deviceKey;
    String _payload;
    int _httpCode;
};
#endif