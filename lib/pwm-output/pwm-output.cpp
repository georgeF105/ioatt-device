#include "pwm-output.h"
#include <Arduino.h>
#include <device-status.h>

PWMOutput::PWMOutput (int updateRate, int outputPin, DeviceStatus *deviceStatus) {
  _updateRate = updateRate;
  _lastUpdated = millis();
  _outputPin = outputPin;
  _deviceStatus = deviceStatus;
  pinMode(_outputPin, OUTPUT);
}

boolean PWMOutput::update () {
  if (_lastUpdated + _updateRate < millis()) {
    _lastUpdated = millis();
    setOutput();
    return true;
  }
  return false;
}

void PWMOutput::setOutput () {
  int pwmValue = _deviceStatus->getInt("pwmValue");

  analogWrite(_outputPin, pwmValue);
}