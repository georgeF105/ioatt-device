#include "ota-update.h"
#include "config.h"

#include <Arduino.h>
#include <ESP8266httpUpdate.h>

OTAUpdate::OTAUpdate(const char *type) {
  _type = type;
  _macAddress = WiFi.macAddress();
}

void OTAUpdate::checkForUpdate () {
    t_httpUpdate_return ret = ESPhttpUpdate.update(REMOTE_SERVER_ADDRESS, 3000, String("/firmware/") + String(_type) + String(".bin"), _currentVersion);
  
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;
  
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;
  
        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            break;
    }
}
