#ifndef CONFIG_H
#define CONFIG_H

#ifdef SONOFF_PLUG
#define TYPE "sonoff_plug"
#endif

#define INPUT_BUTTON_PIN 0
#define INPUT_BUTTON_ON LOW
#define INPUT_BUTTON_OFF HIGH

#define DHT_PIN 2

#ifdef D1_MINI_DHT
#define TYPE "d1_mini"
#define USE_DHT_SENSOR
#endif

#define REMOTE_SERVER_ADDRESS "192.168.1.25"

#endif