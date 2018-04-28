#include "pwm-output.h"
#include <Arduino.h>
#include <device-status.h>

PWMOutput::PWMOutput (int updateRate, int outputPin, DeviceStatus *deviceStatus, int increaseRate, int maxValue) {
  _updateRate = updateRate;
  _lastUpdated = millis();
  _outputPin = outputPin;
  _deviceStatus = deviceStatus;
  pinMode(_outputPin, OUTPUT);
  _increaseRate = increaseRate;
  _maxValue = maxValue;
  _pwmValue = 0;
  analogWriteRange(1023);
  analogWrite(_outputPin, _pwmValue);
  _previousState = false;
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
  int newPWMValue = _deviceStatus->getInt("boolState");

  if (newPWMValue > _pwmValue && newPWMValue <=_maxValue) {
    _pwmValue++;
  }

  if (newPWMValue < _pwmValue && newPWMValue >= 0) {
    _pwmValue--;
  }

  // if (newPWMValue && _pwmValue < _maxValue) {
  //   if (_currentUpdateRate > 100) {
  //     _currentUpdateRate = _currentUpdateRate - _increaseRate;
  //   }
  //   // _currentUpdateRate = _currentUpdateRate / 1.05;
  //   _pwmValue++; // _increaseRate;
  // }

  // if (!newPWMValue && _pwmValue > 0) {
  //   if (_currentUpdateRate > 100) {
  //     _currentUpdateRate = _currentUpdateRate - _increaseRate;
  //   }
  //   // _currentUpdateRate = _currentUpdateRate * 1.05;
  //   _pwmValue--;
  // }
  // Serial.print("UpdateRate");
  // Serial.println(_currentUpdateRate);
  Serial.print("pwmValue: ");
  Serial.println(_pwmValue);
  analogWrite(_outputPin, _pwmValue);
}