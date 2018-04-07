#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include <Arduino.h>

class DeviceConfig {
  public:
    DeviceConfig (void);
    boolean fetch ();
    boolean configured = false;
    boolean hasDHTSensor = false;
    boolean hasPWMLight = false;
    boolean hasSonoffPlug = false;
};

#endif