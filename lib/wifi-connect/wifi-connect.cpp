#include "wifi-connect.h"

#include "storage.h"

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

WifiConnect::WifiConnect (Storage storage) {
  _storage = storage;
}

boolean WifiConnect::connect () {
  String deviceKey = _storage.getDeviceKey();
  boolean isDeviceKeyValid = !deviceKey.equals(DEVICE_KEY_ERROR);

  if (isDeviceKeyValid) {
    WiFiManager wifiManager;
    wifiManager.setTimeout(180); // 3 min
    return wifiManager.autoConnect("IOATT-autoConnect");
  } else {
    WiFiManagerParameter deviceKeyParameter("deviceKey", "unique key", "", DEVICE_KEY_MAX_LENGTH);
    WiFiManager wifiManager;
    wifiManager.addParameter(&deviceKeyParameter);
    wifiManager.setTimeout(180); // 3 min

    boolean successfullyConnected = wifiManager.startConfigPortal("IOATT-onDemand");

    if (successfullyConnected) {
      int deviceKeyLength = deviceKeyParameter.getValueLength();
      char newDeviceKey[DEVICE_KEY_MAX_LENGTH];
      strcpy(newDeviceKey, deviceKeyParameter.getValue());
      _storage.setDeviceKey(newDeviceKey, deviceKeyLength);
      return true;
    }

    return false;
  }
}