#include <Arduino.h>

#include <ota-update.h>
#include <wifi-connect.h>
#include <storage.h>

#ifdef USE_DHT_SENSOR
#include <dht-sensor.h>
DHTSensor dhtSensor(30000); // update every 30 sec
#endif

#define INPUT_BUTTON_PIN 0
#define INPUT_BUTTON_ON LOW
#define INPUT_BUTTON_OFF HIGH

#define DHT_PIN 2

unsigned long lastOutputPollTime;

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

    #ifdef USE_DHT_SENSOR
    dhtSensor.init();
    #endif
}

void loop() {
    #ifdef USE_DHT_SENSOR
        if(dhtSensor.update()) {
            Serial.println("Updated sensor!");
        }
    #endif

    delay(10);
}