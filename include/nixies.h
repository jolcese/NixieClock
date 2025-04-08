#include <Arduino.h>
#include <config.h>

#define SHIFT_REGISTER_ARRAY_SIZE 72
#define TPIC6595_DELAY_uS 1

extern globalConfig config;

void initNixies (uint8_t serial_pin, uint8_t shift_register_clock_pin, uint8_t register_clock_pin, uint8_t hv_enable_pin, bool debug);
void setNixiesMode(uint8_t mode);
void displayNixies (uint8_t hour, uint8_t minute, uint8_t second);

void digitsToBitArray(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4, uint8_t digit5, uint8_t digit6, uint8_t *shiftRegisterBitArray);
void shiftOut_tpic6595(uint8_t *data, uint8_t size);
void digitToBitArray(uint8_t digit, uint8_t bit_array[10]) ;
void reverseBitsOnEachByte(uint8_t *data, size_t size);
