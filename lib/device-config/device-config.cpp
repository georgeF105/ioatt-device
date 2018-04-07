#include "device-config.h"
#include <Arduino.h>

DeviceConfig::DeviceConfig () {
}

boolean DeviceConfig::fetch () {
  configured = true;
  hasDHTSensor = false;
  hasPWMLight = true;
  hasSonoffPlug = false;
  return true;
}