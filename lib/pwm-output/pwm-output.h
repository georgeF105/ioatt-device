#ifndef PWM_OUTPUT_H
#define PWM_OUTPUT_H

#include <Arduino.h>
#include <device-status.h>

class PWMOutput {
  public:
    PWMOutput (int updateRate, int outputPin, DeviceStatus *deviceStatus, int increaseRate, int maxValue);
    boolean update ();

  private:
    int _updateRate;
    int _lastUpdated;
    int _outputPin;
    int _pwmValue;
    void setOutput ();
    DeviceStatus* _deviceStatus;
    int _increaseRate;
    int _maxValue;
    int _previousState;
    int _currentUpdateRate;
};

#endif