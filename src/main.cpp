#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <FirebaseArduino.h>
#include <EEPROM.h>

#include <ioatt-device.h>

// #include "config.h"

#ifdef SONOFF_PLUG
#define TYPE "sonoff_plug"
#endif

#ifdef D1_MINI_DHT
// #include <Adafruit_Sensor.h>
#include <DHT.h>
#define TYPE "d1_mini"
#define DHT_SENSOR_TYPE "DHT"
#define DHT_PIN 2
// #define DHT_TYPE DHT22
unsigned long lastSensorPollTime;
// DHT dht (DHT_PIN, DHT_TYPE);
DHT dht;
#endif

unsigned long lastOutputPollTime;

IOATTDevice ioattDevice (VERSION, TYPE, FIREBASE_HOST, FIREBASE_AUTH);

SensorConfig sensorConfig;
OutputConfig outputConfig;

#define INPUT_BUTTON_PIN 0
#define INPUT_BUTTON_ON LOW
#define INPUT_BUTTON_OFF HIGH
boolean currentOutputState;

void setup() {
    Serial.begin(115200);
    Serial.print("Starting Version: ");
    Serial.print(VERSION);
    Serial.print(" Type: ");
    Serial.println(TYPE);

    pinMode(INPUT_BUTTON_PIN, INPUT_PULLUP);

    delay (500);
    ioattDevice.startUp();
    outputConfig = ioattDevice.getOutputConfig();

    #ifdef D1_MINI_DHT
    sensorConfig = ioattDevice.getSensorConfig();
    Serial.println("Starting dht sensor");
    dht.setup(DHT_PIN);
    #endif

    Serial.println("done setup");
    Serial.print("SensorConfig.type: ");
    Serial.println(sensorConfig.type);
    Serial.print("SensorConfig.pollRate: ");
    Serial.println(sensorConfig.pollRate);

    Serial.print("outputConfig.pollRate: ");
    Serial.println(outputConfig.pollRate);
    Serial.print("outputConfig.pin: ");
    Serial.println(outputConfig.pin);
    pinMode(outputConfig.pin, OUTPUT);
}

#ifdef D1_MINI_DHT
void checkAndPushSensorData () {
    if (lastSensorPollTime + sensorConfig.pollRate < millis()) {
        lastSensorPollTime = millis();

        if (sensorConfig.type == DHT_SENSOR_TYPE) {
            Serial.println("getting and pushing sensor data");
            ioattDevice.pushSensorData(dht.getTemperature(), dht.getHumidity());
        }
    }
}
#endif

void setOutputState (boolean state) {
    currentOutputState = state;
    if (state) {
        Serial.println("Setting pin to on");
        digitalWrite(outputConfig.pin, INPUT_BUTTON_ON);
        ioattDevice.setDeviceActualValue(true);
    } else {
        Serial.println("Setting pin to off");
        digitalWrite(outputConfig.pin, INPUT_BUTTON_OFF);
        ioattDevice.setDeviceActualValue(false);
    }
}

void pollAndUpdateOutputs () {
    if (lastOutputPollTime + outputConfig.pollRate < millis()) {
        lastOutputPollTime = millis();
        setOutputState(ioattDevice.getOutputTargetValue());
    }
}

void loop() {
    if (digitalRead(INPUT_BUTTON_PIN) == INPUT_BUTTON_ON) {
        Serial.println("Button pushed");
        currentOutputState = !currentOutputState;
        setOutputState(currentOutputState);
        ioattDevice.setDeviceValue(currentOutputState);
    }

    pollAndUpdateOutputs();

    #ifdef D1_MINI_DHT
    checkAndPushSensorData();
    #endif

    delay(10);
}