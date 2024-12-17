#include <WiFi.h>
#include <WiFiManager.h>

extern bool debug_wifi;
extern bool gFirstBoot;
extern WiFiManager gWifiManager;

void WiFiEvent(WiFiEvent_t event);
void setupWifiManager();
