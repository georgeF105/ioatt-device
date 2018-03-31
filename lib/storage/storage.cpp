#include "storage.h"

#include <Arduino.h>
#include <EEPROM.h>

Storage::Storage () {
  EEPROM.begin(512);
}

String Storage::getDeviceKey () {
    byte deviceKeyLength = EEPROM.read(DEVICE_KEY_LENGTH_ADDRESS);
    char deviceKey[DEVICE_KEY_MAX_LENGTH];

    if (deviceKeyLength < DEVICE_KEY_MIN_LENGTH || deviceKeyLength > DEVICE_KEY_MAX_LENGTH) {
      return DEVICE_KEY_ERROR;
    }
    for(int i = 0; i < deviceKeyLength; i++) {
      deviceKey[i] = EEPROM.read(i + DEVICE_KEY_ADDRESS);
    }

    return String(deviceKey);
}

void Storage::setDeviceKey (const char *key, int length) {
  for(int i = 0; i < length; i++) {
    Serial.print(key[i]);
    EEPROM.write(i + DEVICE_KEY_ADDRESS, key[i]);
  }
  EEPROM.write(DEVICE_KEY_LENGTH_ADDRESS, length);
  EEPROM.commit();
}