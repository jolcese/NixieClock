// ****************************************************************************
//
// Nixie Display
//
// Based on ESP32-C3, Relay for turning on/off the HV power supply (170Vdc)
//
// ****************************************************************************

#include <Arduino.h>
#include <config.h>
#include <storage.h>

// #include "FieldTypes.h"

// ****************************************************************************
//
// HARDWARE CONFIGURATION
//
// ****************************************************************************

#define LED_PIN               0 // Yellow
#define LED_COUNT             6

#define NIXIE_SERIAL_IN_PIN   1 // Green
#define NIXIE_RCK_PIN         2 // Blue
#define NIXIE_SRCK_PIN        3 // White
#define NIXIE_HV_ENABLE_PIN   4 // Yellow

// ****************************************************************************
//
// Global variables and Objects
//
// ****************************************************************************

bool gInterrupted = false;

globalConfig config;

// uint8_t ledMode = 1; // 0 - Rainbow / 1 - Fixed Orange
// uint8_t nixieMode = 1; // 0 - Off / 1 - Clock / 2 - all digits 0-9 / 3 - every individual bit


// Debug levels
#define DEBUG                 0
#define DEBUG_WIFI            0
#define DEBUG_TIME            0
#define DEBUG_SHIFT_REGISTER  0
#define DEBUG_EEPROM          0

// Displays
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel backlightLeds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
#include <leds.h>
#include <nixies.h>

// Timer
hw_timer_t *timer = NULL;

// Wifi
bool debug_wifi = DEBUG_WIFI;
#include <wifi-helpers.h>
WiFiManager gWifiManager;
bool gFirstBoot = true;

// NTP
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
#include <NTPClient.h>
NTPClient timeClient(ntpUDP);
int gOffset = -28800; // -8 Hs

// API
#include <api.h>

// ****************************************************************************
//
// ISR
//
// ****************************************************************************

// void IRAM_ATTR onTimer() { 
void ARDUINO_ISR_ATTR onTimer() { 
  gInterrupted = true;
}

// ****************************************************************************
//
// Setup
//
// ****************************************************************************
void setup() {

  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("Nixie Clock - (c) 2025 Jose Olcese");
  Serial.println();

  uint8_t *bytePtr = (uint8_t*)&config;
  for (uint8_t i = 0 ; i < sizeof(config) ; i++) {
    bytePtr[i] = 0;
  }

  storageInit(DEBUG_EEPROM);

  // Setup Nixies
  initNixies(NIXIE_SERIAL_IN_PIN, NIXIE_SRCK_PIN, NIXIE_RCK_PIN, NIXIE_HV_ENABLE_PIN, DEBUG_SHIFT_REGISTER);
  displayNixies(0, 0, 0);
  setNixiesMode(config.nixieMode);

  // Setup LEDS
  initLeds(&backlightLeds, LED_PIN);

  // Setup Wifi
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP   
  WiFi.onEvent(WiFiEvent);

  // Reset Wifi setting if OnOff pressed for 5 seconds
  // if (digitalRead(buttonOnOffPin) == LOW) {
  //   delay(5000);
  //   if (digitalRead(buttonOnOffPin) == LOW) {     
  //     gWifiManager.resetSettings();
  //     Serial.println("Wifi reset!");
  //   }
  // }
  setupWifiManager();
  setupApis();
  
  // Setup ISR with 5Hz frequency
  timer = timerBegin(1000000); // 
  timerAttachInterrupt(timer, &onTimer); 
  timerAlarm(timer, 200000, true, 0);

}

void everyOneSecond() {

  static uint8_t value1 = 0;
  static uint8_t value2 = 0;

  if (DEBUG) {
  Serial.printf("nixieMode: %d\n", config.nixieMode);
  }
  // Serial.printf("value1: %d - value2: %d\n", value1, value2);

  // int hours = timeClient.getHours();
  // int minutes = timeClient.getMinutes();
  // int seconds = timeClient.getSeconds();
  
  timeClient.setTimeOffset(gOffset);

  switch (config.nixieMode) {
    case 0:
      break;

    case 1:
      // if (hours + gOffset > 0) {
      //   hours = hours + gOffset;
      // } else {
      //   hours = 24 + ( hours + gOffset );
      // }
      if (DEBUG_TIME) {
        Serial.printf("Local Time: %02d:%02d:%02d - Timezone Offset: %ld seconds\n", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), gOffset);
        // Serial.printf("Time: %02d:%02d:%02d UTC - %02d:%02d:%02d (%d)\n", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), hours, minutes, seconds, gOffset);
      }
      displayNixies(timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
      break;

    case 2: 
      displayNixies(value1 * 11, value1 * 11, value1 * 11);
      value1++;
      if (value1 == 10) value1 = 0;
      break;

    case 3:
      uint8_t localShiftRegisterBitArray[SHIFT_REGISTER_ARRAY_SIZE];
      if (value2 == 4) {
        for (int temp = 0 ; temp < 72; temp ++) {
          localShiftRegisterBitArray[temp] = 0;
        }
        localShiftRegisterBitArray[value1] = 1;
        reverseBitsOnEachByte(localShiftRegisterBitArray,  SHIFT_REGISTER_ARRAY_SIZE);
        shiftOut_tpic6595(localShiftRegisterBitArray,  SHIFT_REGISTER_ARRAY_SIZE);

        value1++;
        if (value1 == 72) value1 = 0;
      }
      value2++;
      if (value2 == 5) value2 = 0;
      break;

    default:
      break;
  }
}

// ****************************************************************************
//
// MainLoop
//
// ****************************************************************************

void loop() {
  while (WiFi.status() != WL_CONNECTED && gFirstBoot == false )
  {
    if (DEBUG_WIFI) Serial.println("Reconnect Wifi!");
    setupWifiManager();
  }
  gWifiManager.process();
  handleApis();

  timeClient.update();

  if (gInterrupted == true) {
    gInterrupted = false;
    everyOneSecond();
  }

  switch (config.ledMode) {
    case 0:
      colorWipe(backlightLeds.Color(0, 0, 0)); // Off
      delay(20);
      break;
    case 1:
      colorWipe(backlightLeds.Color(255, 75, 0)); // Orange
      delay(20);
      break;
    case 2:
      rainbow();             // Flowing rainbow cycle along the whole strip
      delay(20);
      break;
    default:
      delay(20);
      break;
  }
}
