// *****************************************
// Storage Header
// *****************************************

#include <arduino.h>
#include <EEPROM.h>
#include <config.h>

extern globalConfig config;

#define STORAGE_MAGICAL           161
#define STORAGE_INIT                0

// uint8_t storageWrite(String name);
// uint8_t storageRead(String name);
// uint8_t setValueFromStorage(String name);
// uint8_t initStorageFromValue(String name);
void storageInit(bool debug);
void storageConfigRead();
void storageConfigWrite();
