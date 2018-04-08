#ifndef PWM_OUTPUT_H
#define PWM_OUTPUT_H

#include <Arduino.h>
#include <device-status.h>

class PWMOutput {
  public:
    PWMOutput (int updateRate, int outputPin, DeviceStatus *deviceStatus);
    boolean update ();

  private:
    int _updateRate;
    int _lastUpdated;
    int _outputPin;
    void setOutput ();
    DeviceStatus* _deviceStatus;
};

#endif