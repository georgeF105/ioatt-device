#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <Arduino.h>

#include "storage.h"

class WifiConnect {
  public:
    WifiConnect (Storage storage);
    boolean connect ();

  private:
    Storage _storage;
};

#endif