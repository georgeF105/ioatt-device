#include "dht-sensor.h"

#include <Arduino.h>
#include <config.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

DHTSensor::DHTSensor (int updateRate) {
  _updateRate = updateRate;
  _lastUpdated = millis();
}

void DHTSensor::init () {
  _dht.setup(DHT_PIN);
}

boolean DHTSensor::update () {
  if (_lastUpdated + _updateRate < millis()) {
    _lastUpdated = millis();
    checkAndPushState();
    return true;
  }
  return false;
}

void DHTSensor::checkAndPushState () {
  pushSensorState(getSensorState());
}

DHTSensorState DHTSensor::getSensorState () {
  DHTSensorState state;
  state.temperature = _dht.getTemperature();;
  state.humidity = _dht.getHumidity();
  return state;
}

void DHTSensor::pushSensorState (DHTSensorState state) {
  HTTPClient http;
  String url = String("http://") + String(REMOTE_SERVER_ADDRESS) + String(":3000/sensor");
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  String payload = String("{\"temperature\":") + String(state.temperature) + String(",\"humidity\":") + String(state.humidity) + String("}");
  http.POST(payload);
  http.writeToStream(&Serial);
  http.end();
}