#include <Arduino.h>

#ifndef CONFIG_H
#define CONFIG_H

struct globalConfig{

  uint8_t nixieMode;
  uint8_t ledMode;
  uint8_t timerEnable;
  uint8_t onHour;
  uint8_t onMinute;
  uint8_t offHour;
  uint8_t offMinute;
  int8_t  tz;
};


// struct Field {
//   String name;
//   String label;
//   String type;
//   uint8_t min;
//   uint8_t max;
//   FieldGetter getValue;
//   FieldGetter getOptions;
//   FieldSetter setValue;
//   String pattern;
//   uint8_t eepromIndex;

// };

// typedef Field FieldList[];


//     {"name", "Name", LabelFieldType, 0, 0, getName, NULL, NULL, "all", 255},

#endif // CONFIG_H
