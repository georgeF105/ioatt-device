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

#include <Adafruit_Sensor.h>
#include <DHT.h>

#include "config.h"

#include <ioatt-device.h>


#define CURRENT_VERSION "0.0.3"
#define TYPE "D1_MINI_DHT"

#define DHT_SENSOR_TYPE "DHT"
#define DHT_PIN D4
#define DHT_TYPE DHT22
unsigned long lastSensorPollTime;

#define LED_PIN 2
unsigned long lastOutputPollTime;

IOATTDevice ioattDevice ("0.0.3", TYPE, FIREBASE_HOST, FIREBASE_AUTH);
DHT dht (DHT_PIN, DHT_TYPE);

SensorConfig sensorConfig;
OutputConfig outputConfig;

void setup() {
    Serial.begin(115200);
    Serial.print("Starting Version: ");
    Serial.print(CURRENT_VERSION);
    Serial.print(" Type: ");
    Serial.println(TYPE);

    pinMode(LED_PIN, OUTPUT);

    delay (500);    
    ioattDevice.startUp();
    sensorConfig = ioattDevice.getSensorConfig();
    outputConfig = ioattDevice.getOutputConfig();
    dht.begin();
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

void checkAndPushSensorData () {
    if (lastSensorPollTime + sensorConfig.pollRate < millis()) {
        lastSensorPollTime = millis();

        if (sensorConfig.type == DHT_SENSOR_TYPE) {
            Serial.println("getting and pushing sensor data");
            ioattDevice.pushSensorData(dht.readTemperature(), dht.readHumidity());
        }
    }
}

void pollAndUpdateOutputs () {
    if (lastOutputPollTime + outputConfig.pollRate < millis()) {
        lastOutputPollTime = millis();
        if (ioattDevice.getOutputTargetValue()) {
            Serial.println("Setting pin to high");
            digitalWrite(outputConfig.pin, HIGH);
            ioattDevice.setDeviceActualValue(true);
        } else {
            Serial.println("Setting pin to low");
            digitalWrite(outputConfig.pin, LOW);
            ioattDevice.setDeviceActualValue(false);
        }
    }
}

void loop() {
    pollAndUpdateOutputs();
    checkAndPushSensorData();

    delay(10);
}