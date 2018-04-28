#include <Arduino.h>

#include <ota-update.h>
#include <wifi-connect.h>
#include <storage.h>
#include <device-config.h>
#include <device-status.h>
#include <pwm-output.h>

// #ifdef USE_DHT_SENSOR
#include <dht-sensor.h>
DHTSensor dhtSensor(30000); // update every 30 sec
// #endif

#define INPUT_BUTTON_PIN 0
#define INPUT_BUTTON_ON LOW
#define INPUT_BUTTON_OFF HIGH

#define DHT_PIN 2

#define PWN_PIN 2

int pwmValue = 0;
unsigned long lastOutputPollTime;

boolean currentOutputState;

Storage storage;
WifiConnect wifiConnect (storage);
OTAUpdate otaUpdate (TYPE);
DeviceConfig deviceConfig;
DeviceStatus deviceStatus (10000, &storage);
PWMOutput pwmOutout (1000, PWN_PIN, &deviceStatus, 50, 800);

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
        deviceConfig.fetch();
        deviceStatus.fetchStatus();
    }

    delay (200);

    if (deviceConfig.hasDHTSensor) {
        dhtSensor.init();
    }
}

void loop() {
    if (deviceStatus.update()) {
        Serial.println("Updated status");
    }

    if (deviceConfig.hasDHTSensor) {
        if(dhtSensor.update()) {
            Serial.println("Updated sensor!");
        }
    }

    if (deviceConfig.hasPWMLight) {
        if (pwmOutout.update()) {
            // Serial.println("pwm updated");
        }
    }

    delay(10);
}