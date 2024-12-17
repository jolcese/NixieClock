// ****************************************************************************
//
// Nixies Display
//
// ****************************************************************************

#include <nixies.h>

uint8_t shiftRegisterBitArray[SHIFT_REGISTER_ARRAY_SIZE];
uint8_t nixies_serial_pin;
uint8_t nixies_shift_register_clock_pin;
uint8_t nixies_register_clock_pin;
uint8_t nixies_hv_enable_pin;

bool nixies_debug;

// Initialize HW pins and debug mode
void initNixies (uint8_t serial_pin, uint8_t shift_register_clock_pin, uint8_t register_clock_pin, uint8_t hv_enable_pin, bool debug) {
  pinMode(serial_pin, OUTPUT);
  pinMode(register_clock_pin, OUTPUT);
  pinMode(shift_register_clock_pin, OUTPUT);
  pinMode(hv_enable_pin, OUTPUT);
  
  digitalWrite(serial_pin, LOW );
  digitalWrite(register_clock_pin, LOW );
  digitalWrite(shift_register_clock_pin, LOW );
  digitalWrite(hv_enable_pin, LOW );

  nixies_serial_pin = serial_pin;
  nixies_shift_register_clock_pin = shift_register_clock_pin;
  nixies_register_clock_pin = register_clock_pin;
  nixies_hv_enable_pin = hv_enable_pin;
  nixies_debug = debug;
}

// Enable/Disable HV power supply
void switchNixies(bool isOn) {
   digitalWrite(nixies_hv_enable_pin, isOn );
}

// Display hours/minutes/seconds (00-99 on each pair of digits)
void displayNixies (uint8_t hour, uint8_t minute, uint8_t second) {
  digitsToBitArray(hour / 10, hour % 10, minute / 10, minute % 10, second / 10, second % 10, shiftRegisterBitArray);
  reverseBitsOnEachByte(shiftRegisterBitArray, SHIFT_REGISTER_ARRAY_SIZE);
  shiftOut_tpic6595(shiftRegisterBitArray, SHIFT_REGISTER_ARRAY_SIZE);
}

// Display:        12345678 90dXX123 4567890d   (d - dot inside digit / XX colon between digits)
// Shift Register: 12345678 12345678 12345678

// Function to convert six digits to a 72-bytes array representing the 72 bits
void digitsToBitArray(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4, uint8_t digit5, uint8_t digit6, uint8_t *shiftRegisterBitArray) {
    
    uint8_t tempIndex;
    
    // Initialize all bits to 0
    for (tempIndex = 0; tempIndex < SHIFT_REGISTER_ARRAY_SIZE; tempIndex++) {
        shiftRegisterBitArray[tempIndex] = 0;
    }

    // Convert the first digit to bits 0–9
    uint8_t temp[10];

    // Digits 1 & 2
    digitToBitArray(digit1, temp);
    for (tempIndex = 0; tempIndex < 10; tempIndex++) {
        shiftRegisterBitArray[tempIndex] = temp[tempIndex];
    }

    // Convert the second digit to bits 10–19
    digitToBitArray(digit2, temp);
    for (tempIndex = 0; tempIndex < 10; tempIndex++) {
        shiftRegisterBitArray[tempIndex + 11] = temp[tempIndex];
    }

    // Always on colon
    shiftRegisterBitArray[22] = 1;
    shiftRegisterBitArray[23] = 1;

    // Digits 3 & 4
    digitToBitArray(digit3, temp);
    for (tempIndex = 0; tempIndex < 10; tempIndex++) {
        shiftRegisterBitArray[tempIndex + 24] = temp[tempIndex];
    }

    digitToBitArray(digit4, temp);
    for (tempIndex = 0; tempIndex < 10; tempIndex++) {
        shiftRegisterBitArray[tempIndex + 35] = temp[tempIndex];
    }

    // Always on colon
    shiftRegisterBitArray[46] = 1;
    shiftRegisterBitArray[47] = 1;

    // Digits 5 & 6
    digitToBitArray(digit5, temp);
    for (tempIndex = 0; tempIndex < 10; tempIndex++) {
        shiftRegisterBitArray[tempIndex + 48] = temp[tempIndex];
    }

    digitToBitArray(digit6, temp);
    for (tempIndex = 0; tempIndex < 10; tempIndex++) {
        shiftRegisterBitArray[tempIndex + 59] = temp[tempIndex];
    }

}

// Function to convert a digit to its 10-bit representation
void digitToBitArray(uint8_t digit, uint8_t bit_array[10]) {
    // Initialize all bits to 0
    for (int i = 0; i < 10; i++) {
        bit_array[i] = 0;
    }

    // Set the corresponding bit for the digit
    if (digit >= 1 && digit <= 9) {
        bit_array[digit - 1] = 1; // Set the (digit-1)th bit
    } else if (digit == 0) {
        bit_array[9] = 1; // Set the 10th bit for digit 0
    }
}

// Function to revert each byte on the array
void reverseBitsOnEachByte(uint8_t *data, size_t size) {
    // Ensure the size is a multiple of 8
    if (size % 8 != 0) {
        return;
    }
    // Process each 8-byte chunk
    for (size_t i = 0; i < size; i += 8) {
        // Reverse the 8-byte chunk
        for (size_t j = 0; j < 4; j++) {
            // Swap the jth element with the (7-j)th element within the chunk
            uint8_t temp = data[i + j];
            data[i + j] = data[i + 7 - j];
            data[i + 7 - j] = temp;
        }
    }
}

// Function to ouput an array of bytes
void shiftOut_tpic6595(uint8_t *data, uint8_t size) {
  
  if (nixies_debug) {
    Serial.print("Output: ");
  }
  
  for (int bitIndex = 0 ; bitIndex < size ; bitIndex++) {

    bool bit = data[bitIndex] & 0x01;

    if (nixies_debug) {
      if (bit == true) {
        Serial.printf("1");
      } else {
        Serial.printf("0");
      }
    }

    // Write bit of data on serial_pin
    digitalWrite(nixies_serial_pin, bit );
    delayMicroseconds(TPIC6595_DELAY_uS);

    digitalWrite(nixies_shift_register_clock_pin, HIGH);
    delayMicroseconds(TPIC6595_DELAY_uS);
    digitalWrite(nixies_shift_register_clock_pin, LOW);
    delayMicroseconds(TPIC6595_DELAY_uS);

  }

  digitalWrite(nixies_serial_pin, LOW);
  delayMicroseconds(TPIC6595_DELAY_uS);

  digitalWrite(nixies_register_clock_pin, HIGH);
  delayMicroseconds(TPIC6595_DELAY_uS);
  digitalWrite(nixies_register_clock_pin, LOW);
  delayMicroseconds(TPIC6595_DELAY_uS);

  if (nixies_debug) {
    Serial.println(" Latch");
  }
}
