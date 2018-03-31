#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

#define DEVICE_KEY_ADDRESS 4
#define DEVICE_KEY_MIN_LENGTH 5
#define DEVICE_KEY_MAX_LENGTH 12
#define DEVICE_KEY_LENGTH_ADDRESS 2

#define DEVICE_KEY_ERROR "ERROR"

struct response {
  String response;
  String error;
  boolean hasError;
};

class Storage {
  public:
    Storage (void);
    String getDeviceKey ();
    void setDeviceKey (const char *key, int length);
};

#endif