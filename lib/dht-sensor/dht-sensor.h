#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <Arduino.h>
#include <DHT.h>

struct DHTSensorState {
  float temperature;
  float humidity;
};

class DHTSensor {
  public: 
    DHTSensor (int updateRate);
    void init ();
    void update ();
    DHTSensorState getSensorState ();
    void checkAndPushState ();

  private:
    int _updateRate;
    int _lastUpdated;
    DHT _dht;
    void pushSensorState(DHTSensorState state);
};

#endif