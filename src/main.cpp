#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <FirebaseArduino.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <EEPROM.h>

#define LED_PIN 2
#define USE_SERIAL Serial

void setup() {
    USE_SERIAL.begin(115200);
    USE_SERIAL.println("Starting");
    // initialize digital pin LED_PIN as an output.
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    USE_SERIAL.println("Setting pin to high");
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    
    USE_SERIAL.println("Setting pin to low");
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
}