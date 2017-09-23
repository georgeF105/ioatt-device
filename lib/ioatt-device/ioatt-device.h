
#ifndef IOATT_H
#define IOATT_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <FirebaseArduino.h>

#define BUILD_SERVER_ADDRESS "192.168.1.25"
#define MAC_ADDRESS_NOT_SET "NOT_SET"

#define DEVICE_KEY_ADDRESS 4
#define DEVICE_KEY_MIN_LENGTH 5
#define DEVICE_KEY_MAX_LENGTH 12
#define DEVICE_KEY_LENGTH_ADDRESS 2

struct SensorConfig {
  String type;
  int pollRate;
};

struct OutputConfig {
  int pin;
  int pollRate;
};

class IOATTDevice {
  public:
    IOATTDevice(const char *version, const char *type, const char *firebaseHost, const char *firebaseAuth);
    void startUp (void);
    String deviceKey();
    boolean getOutputTargetValue();
    void setDeviceActualValue(boolean value);
    SensorConfig getSensorConfig ();
    void pushSensorData (float temperature, float humidity);
    OutputConfig getOutputConfig ();

  private:
    const char *_version;
    const char *_type;
    const char *_firebaseHost;
    const char *_firebaseAuth;
    char _deviceKey[DEVICE_KEY_MAX_LENGTH];
    byte _deviceKeyLength;
    String _macAddress;
    boolean _shouldSaveConfig = false;
    OutputConfig _outputConfig;
    StaticJsonBuffer<200> _jsonBuffer;
    JsonObject* _currentSensorData;
    // void saveConfigCallback ();
    boolean fetchDeviceKeyFromEEPROM (void);
    void saveDeviceKeyToEEPROM ();
    void verifyDeviceMacAddress ();
    String getRemoteMacAddress ();
    void wipeDeviceKeyAndReboot (void);
    void checkForUpdates (void);
    void checkForFirebaseError ();
};

#endif