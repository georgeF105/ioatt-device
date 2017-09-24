#include "ioatt-device.h"

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

IOATTDevice::IOATTDevice(const char *version, const char *type, const char *firebaseHost, const char *firebaseAuth) {
    _version = version;
    _type = type;
    _firebaseHost = firebaseHost;
    _firebaseAuth = firebaseAuth;
    _macAddress = WiFi.macAddress();
    _shouldSaveConfig = false;

    JsonObject& currentSensorData = _jsonBuffer.createObject();
    _currentSensorData = &currentSensorData;
}

void IOATTDevice::startUp(void) {
    EEPROM.begin(512);

    if (fetchDeviceKeyFromEEPROM()) {
        WiFiManager wifiManager;
        wifiManager.autoConnect("IOATT-autoConnect");
      } else {
        WiFiManagerParameter _device_key("deviceKey", "unique key", "", DEVICE_KEY_MAX_LENGTH);
        WiFiManager wifiManager;
        wifiManager.addParameter(&_device_key);
    
        wifiManager.startConfigPortal("IOATT-onDemand");
        _deviceKeyLength = _device_key.getValueLength();
        strcpy(_deviceKey, _device_key.getValue());
        saveDeviceKeyToEEPROM();
      }
    
      delay(500);

      Serial.println("Connected to WiFi!!");
      
      Firebase.begin(_firebaseHost, _firebaseAuth);
      delay (500);
      if (Firebase.failed()) {
          Serial.print("setup failed:");
          Serial.println(Firebase.error());  
      }
      verifyDeviceMacAddress();
      checkForUpdates();
}

String IOATTDevice::deviceKey() {
    return String(_deviceKey);
}

SensorConfig IOATTDevice::getSensorConfig() {
    SensorConfig config;
    config.type = Firebase.getString(String("devices/") + String(_deviceKey) + String("/sensor/config/type"));
    config.pollRate = Firebase.getInt(String("devices/") + String(_deviceKey) + String("/sensor/config/pollRate"));
    return config;
}

OutputConfig IOATTDevice::getOutputConfig() {
    _outputConfig.pollRate = Firebase.getInt(String("devices/") + String(_deviceKey) + String("/outputs/config/pollRate"));
    _outputConfig.pin = Firebase.getInt(String("devices/") + String(_deviceKey) + String("/outputs/config/pin"));
    return _outputConfig;
}

void IOATTDevice::pushSensorData(float temperature, float humidity) {    
    _currentSensorData->set("temperature", temperature);
    _currentSensorData->set("humidity", humidity);
    Firebase.push(String("devices/") + String(_deviceKey) + String("/sensor/data"), JsonVariant(*_currentSensorData));
    checkForFirebaseError();
}

boolean IOATTDevice::getOutputTargetValue() {
  return Firebase.getBool(String("devices/") + String(_deviceKey) + String("/outputs/state/targetValue"));
}

void IOATTDevice::setDeviceActualValue(boolean value) {
    Firebase.setBool(String("devices/") + String(_deviceKey) + String("/outputs/state/actualValue"), value);
}

void IOATTDevice::setDeviceValue(boolean value) {
    Firebase.setBool(String("devices/") + String(_deviceKey) + String("/outputs/state/actualValue"), value);
    Firebase.setString(String("devices/") + String(_deviceKey) + String("/outputs/state/setBy"), "DEVICE");
}

boolean IOATTDevice::fetchDeviceKeyFromEEPROM () {
    _deviceKeyLength = EEPROM.read(DEVICE_KEY_LENGTH_ADDRESS);
    if (_deviceKeyLength < DEVICE_KEY_MIN_LENGTH || _deviceKeyLength > DEVICE_KEY_MAX_LENGTH) {
      Serial.print("ERROR: Device Key Wrong Length: ");
      Serial.println(_deviceKeyLength);
      return false;
    }
    for(int i = 0; i < _deviceKeyLength; i++) {
      _deviceKey[i] = EEPROM.read(i + DEVICE_KEY_ADDRESS);
    }
  
    Serial.print("Device key: ");
    Serial.println(_deviceKey);
    return true;
}

void IOATTDevice::saveDeviceKeyToEEPROM () {
    for(int i = 0; i < _deviceKeyLength; i++) {
      EEPROM.write(i + DEVICE_KEY_ADDRESS, _deviceKey[i]);
    }
    Serial.print("Saving deivcekeyLength");
    Serial.println(_deviceKeyLength);

    EEPROM.write(DEVICE_KEY_LENGTH_ADDRESS, _deviceKeyLength);
    EEPROM.commit();
}

void IOATTDevice::verifyDeviceMacAddress () {
    String remoteMacAddress = getRemoteMacAddress();
    checkForFirebaseError();

    if (remoteMacAddress == MAC_ADDRESS_NOT_SET) {
      Firebase.setString(String("devices/") + String(_deviceKey) + String("/macAddress"), _macAddress);
      return;
    }
  
    if (_macAddress != remoteMacAddress) {
      Serial.println("Error: MAC Address does not match.");
      wipeDeviceKeyAndReboot();
    }
}

String IOATTDevice::getRemoteMacAddress () {
    return Firebase.getString(String("devices/") + String(_deviceKey) + String("/macAddress"));
}

void IOATTDevice::wipeDeviceKeyAndReboot () {
    Serial.println("Wiping saved Mac Address and rebooting");
    EEPROM.write(DEVICE_KEY_LENGTH_ADDRESS, 0);
    EEPROM.commit();
    ESP.reset();
}

void IOATTDevice::checkForUpdates () {
    Serial.println("checking for updates");
    t_httpUpdate_return ret = ESPhttpUpdate.update(BUILD_SERVER_ADDRESS, 3000, String("/firmware/") + String(_type) + String(".bin"), _version);
  
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;
  
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;
  
        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            break;
    }
}

void IOATTDevice::checkForFirebaseError () {
  if (Firebase.failed()) {
    Serial.print("Error - firebase: ");
    Serial.println(Firebase.error());  
  }
}
