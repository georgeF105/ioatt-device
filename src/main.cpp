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

#include <DHT.h>

#include "config.h"

#include <ioatt-device.h>


#define CURRENT_VERSION "0.0.3"
#define TYPE "D1_MINI_DHT"

#define LED_PIN 2
#define DHT_PIN D4
#define DHT_TYPE DHT22

#define POLL_REFRESH_RATE 20000
unsigned long lastPollTime;

IOATTDevice ioattDevice ("0.0.3", TYPE, FIREBASE_HOST, FIREBASE_AUTH);
DHT dht (DHT_PIN, DHT_TYPE);

void setup() {
    Serial.begin(115200);
    Serial.print("Starting Version: ");
    Serial.print(CURRENT_VERSION);
    Serial.print(" Type: ");
    Serial.println(TYPE);

    pinMode(LED_PIN, OUTPUT);

    delay (500);    
    ioattDevice.startUp();
    dht.begin();
    Serial.println("done setup");
}

void loop() {
    if (ioattDevice.getDeviceKeyValue("led")) {
        Serial.println("Setting pin to high");
        digitalWrite(LED_PIN, HIGH);
    } else {
        Serial.println("Setting pin to low");
        digitalWrite(LED_PIN, LOW);
    }

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    Serial.print("Current temperature: ");
    Serial.println(temperature);
    Serial.print("Current humidity: ");
    Serial.println(humidity);

    delay(10000);
}