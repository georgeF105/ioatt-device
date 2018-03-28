#ifndef CONFIG_H
#define CONFIG_H


#ifdef SONOFF_PLUG
#define TYPE "sonoff_plug"
#endif

#ifdef D1_MINI_DHT
#define TYPE "d1_mini"
#define USE_DHT_SENSOR
#endif

#define REMOTE_SERVER_ADDRESS "192.168.1.25"
#define DHT_PIN 2

#endif