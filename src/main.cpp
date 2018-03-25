#include <Arduino.h>

#include "config.h"
#include <ota-update.h>
#include <wifi-connect.h>
#include <storage.h>

#ifdef SONOFF_PLUG
#define TYPE "sonoff_plug"
#endif

#ifdef D1_MINI_DHT
#include <dht-sensor.h>
// #include <DHT.h>
#define TYPE "d1_mini"
#define DHT_SENSOR_TYPE "DHT"
unsigned long lastSensorPollTime;
DHTSensor dhtSensor(4000);
// DHT dht;
#endif

unsigned long lastOutputPollTime;

// IOATTDevice ioattDevice (VERSION, TYPE, FIREBASE_HOST, FIREBASE_AUTH);

// SensorConfig sensorConfig;
// OutputConfig outputConfig;

#define INPUT_BUTTON_PIN 0
#define INPUT_BUTTON_ON LOW
#define INPUT_BUTTON_OFF HIGH
boolean currentOutputState;

Storage storage;
WifiConnect wifiConnect (storage);
OTAUpdate otaUpdate (TYPE);

boolean hasWifiConnection;

void setup() {
    Serial.begin(115200);
    Serial.print("Starting Version: ");
    Serial.print(VERSION);
    Serial.print(" Type: ");
    Serial.println(TYPE);

    pinMode(INPUT_BUTTON_PIN, INPUT_PULLUP);

    hasWifiConnection = wifiConnect.connect();
    if (hasWifiConnection) {
        Serial.println("connected");
        otaUpdate.checkForUpdate();
    }


    delay (500);

    #ifdef D1_MINI_DHT
    dhtSensor.init();
    #endif
}

void loop() {
    #ifdef D1_MINI_DHT
        dhtSensor.update();
    #endif

    delay(10);
}