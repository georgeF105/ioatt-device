#ifndef DEVICE_STATUS_H
#define DEVICE_STATUS_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define JSON_SIZE 200

class DeviceStatus {
  public:
    DeviceStatus (void);
    void fetchStatus ();
    boolean update ();
    int getInt (char *valueName);

  private:
    int _updateRate;
    int _lastUpdated;
    String _payload;
    int _httpCode;
};
#endif