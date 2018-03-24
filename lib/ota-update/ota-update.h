#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

#include <Arduino.h>

// struct ota_update_return {
//   String message;
//   boolean error;
// };

class OTAUpdate {
  public:
    OTAUpdate (const char *type);
    void checkForUpdate ();
    String getCurrentVersion (void);
  
  private:
    const char *_currentVersion;
    String _macAddress;
    const char *_type;

    void checkForUpdates (void);
};

#endif