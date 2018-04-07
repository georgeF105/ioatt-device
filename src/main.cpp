#include <Arduino.h>

#include <ota-update.h>
#include <wifi-connect.h>
#include <storage.h>
#include <device-config.h>

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

boolean hasWifiConnection;

void setup() {
    Serial.begin(115200);
    Serial.print("Starting Version: ");
    Serial.print(VERSION);
    Serial.print(" Type: ");
    Serial.println(TYPE);

    pinMode(INPUT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(PWN_PIN, OUTPUT);

    hasWifiConnection = wifiConnect.connect();
    if (hasWifiConnection) {
        Serial.println("connected");
        otaUpdate.checkForUpdate();
        deviceConfig.fetch();
    }

    delay (200);

    if (deviceConfig.hasDHTSensor) {
        dhtSensor.init();
    }
}

void loop() {
    if (deviceConfig.hasDHTSensor) {
        if(dhtSensor.update()) {
            Serial.println("Updated sensor!");
        }
    }

    if (deviceConfig.hasPWMLight) {
        pwmValue++;

        if(pwmValue > 255) {
            pwmValue = 0;
        }
        Serial.print("Setting pwm to ");
        Serial.println(pwmValue);

        analogWrite(PWN_PIN, pwmValue);
    }

    delay(100);
}