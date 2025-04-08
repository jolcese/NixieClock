#include <Adafruit_NeoPixel.h>
#include <config.h>

extern globalConfig config;

void initLeds(Adafruit_NeoPixel *backlightLeds, uint8_t serial_pin);
void setLedsMode(uint8_t mode);

void rainbow();
void colorWipe(uint32_t color);

