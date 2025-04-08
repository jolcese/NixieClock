// *****************************************
// Storage
// *****************************************

#include <storage.h>

bool eeprom_debug = false;

// boolean storageNoWriteFirst = true;

// uint8_t storageWrite(String name) {

//   uint8_t eepromIndex = 255;
//   uint8_t value;

//   if (storageNoWriteFirst == true) return(0);

//   for (uint8_t i = 0 ; i < FIELD_COUNT ; i++) {
//     if (fields[i].name == name) {
//       eepromIndex = fields[i].eepromIndex;
//       value = fields[i].getValue().toInt();
//       break;
//     }
//   }
//   if (eepromIndex != 255) {
//     Serial.print("storageWrite(name=");
//     Serial.print(name);
//     Serial.printf(", eepromIndex=%d, value=%d)", eepromIndex, value);
//     Serial.println();
//     Serial.printf("storageWrite: EEPROM.write");
//     Serial.println();
//     EEPROM.write(eepromIndex, value);
//     EEPROM.commit();
//   } else {
//     Serial.printf("storageWrite(name=%s) FIELD NOT FOUND", name);
//     Serial.println();
//   }
// }

// uint8_t storageRead(String name) {

//   uint8_t eepromIndex = 255;
//   uint8_t value = 0;
  
//   for (uint8_t i = 0 ; i < FIELD_COUNT ; i++) {
//     if (fields[i].name == name) {
//       eepromIndex = fields[i].eepromIndex;
//       break;
//     }
//   }
//   if (eepromIndex != 255) {
//     value = EEPROM.read(eepromIndex);
//     Serial.print("storageRead(name=");
//     Serial.print(name);
//     Serial.printf(", eepromIndex=%d, value=%d)", eepromIndex, value);
//     Serial.println();
//   } else {
//     Serial.printf("storageRead(name=%s) FIELD NOT FOUND", name);
//     Serial.println();
//   }

//   return value;
// }

// uint8_t setValueFromStorage(String name) {

//   uint8_t value = storageRead(name);

//   for (uint8_t i = 0 ; i < FIELD_COUNT ; i++) {
//     if (fields[i].name == name) {
//       fields[i].setValue(String(value));
//       break;
//     }
//   }
//   Serial.print("setValueFromStorage(name=");
//   Serial.print(name);
//   Serial.printf(", value=%d)", value);
//   Serial.println();

//   return value;
// }

// uint8_t initStorageFromValue(String name) {

//   uint8_t value;

//   for (uint8_t i = 0 ; i < FIELD_COUNT ; i++) {
//     if (fields[i].name == name) {
//       value = fields[i].getValue().toInt();
//       Serial.print("initStorageFromValue(name=");
//       Serial.print(name);
//       Serial.printf(", value=%d)", value);
//       Serial.println();
//       Serial.printf("initStorageFromValue: EEPROM.write");
//       Serial.println();
//       EEPROM.write(fields[i].eepromIndex, value);
//       EEPROM.commit();
//       break;
//     }
//   }

//   return value;
// }

void storageConfigWrite() {

  uint8_t *bytePtr = (uint8_t*)&config;

  for (uint8_t i = 0 ; i < sizeof(config) ; i++) {
    EEPROM.write(i + 1, bytePtr[i]);
    EEPROM.commit();
    if (eeprom_debug) Serial.printf("Write EEPROM (%d) = %d\n", i + 1, bytePtr[i]);
  }

}

void storageConfigRead() {
  EEPROM.begin(512);

  uint8_t *bytePtr = (uint8_t*)&config;

  if (EEPROM.read(STORAGE_INIT) == STORAGE_MAGICAL) {

    for (uint8_t i = 0 ; i < sizeof(globalConfig) ; i++) {
      bytePtr[i] = EEPROM.read(i + 1);
      if (eeprom_debug) Serial.printf("Read EEPROM (%d) = %d\n", i + 1, bytePtr[i]);
    }

  } else {

    if (eeprom_debug) Serial.println("Initialize EEPROM");

    storageConfigWrite();

    EEPROM.write(0, STORAGE_MAGICAL);
    EEPROM.commit();
  }
}

void storageInit(bool debug) {
  eeprom_debug = debug;
  storageConfigRead();
}