#include <WiFi.h>
#include <wifi-helpers.h>

char gHostName[64];
#define WL_MAC_ADDR_LENGTH 6

void WiFiEvent(WiFiEvent_t event)
{
  if (debug_wifi) Serial.printf("[WiFi-event] event: %d -- ", event);

  switch (event) {
    case ARDUINO_EVENT_WIFI_READY: 
      if (debug_wifi) Serial.println("WiFi interface ready");
      break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
      if (debug_wifi) Serial.println("Completed scan for access points");
      break;
    case ARDUINO_EVENT_WIFI_STA_START:
      if (debug_wifi) Serial.println("WiFi client started");
      break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
      if (debug_wifi) Serial.println("WiFi clients stopped");
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      if (debug_wifi) Serial.println("Connected to access point");
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      if (debug_wifi) Serial.println("Disconnected from WiFi access point");
      // previousWifiReconnectMillis = millis();
      break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
      if (debug_wifi) Serial.println("Authentication mode of access point has changed");
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      if (debug_wifi) Serial.print("Obtained IP address: ");
      if (debug_wifi) Serial.println(WiFi.localIP());
      // previousPollMillis = 0;
      break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
      if (debug_wifi) Serial.println("Lost IP address and IP address is reset to 0");
      break;
    case ARDUINO_EVENT_WPS_ER_SUCCESS:
      if (debug_wifi) Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
      break;
    case ARDUINO_EVENT_WPS_ER_FAILED:
      if (debug_wifi) Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
      break;
    case ARDUINO_EVENT_WPS_ER_TIMEOUT:
      if (debug_wifi) Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
      break;
    case ARDUINO_EVENT_WPS_ER_PIN:
      if (debug_wifi) Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
      break;
    case ARDUINO_EVENT_WIFI_AP_START:
      if (debug_wifi) Serial.println("WiFi access point started");
      break;
    case ARDUINO_EVENT_WIFI_AP_STOP:
      if (debug_wifi) Serial.println("WiFi access point stopped");
      break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
      if (debug_wifi) Serial.println("Client connected");
      break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
      if (debug_wifi) Serial.println("Client disconnected");
      break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
      if (debug_wifi) Serial.println("Assigned IP address to client");
      break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
      if (debug_wifi) Serial.println("Received probe request");
      break;
    case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
      if (debug_wifi) Serial.println("AP IPv6 is preferred");
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
      if (debug_wifi) Serial.println("STA IPv6 is preferred");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP6:
      if (debug_wifi) Serial.println("Ethernet IPv6 is preferred");
      break;
    case ARDUINO_EVENT_ETH_START:
      if (debug_wifi) Serial.println("Ethernet started");
      break;
    case ARDUINO_EVENT_ETH_STOP:
      if (debug_wifi) Serial.println("Ethernet stopped");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      if (debug_wifi) Serial.println("Ethernet connected");
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      if (debug_wifi) Serial.println("Ethernet disconnected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      if (debug_wifi) Serial.println("Obtained IP address");
      break;
    default: 
      break;
  }
}

void setupWifiManager() {

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac);

  sprintf(gHostName, "Nixie-%02X%02X", mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1]);
  if (debug_wifi) Serial.printf("Wifi Manager Setup - Name: %s\n", gHostName );
  if (debug_wifi) Serial.printf("Mac Address: %s\n", WiFi.macAddress() );

  // reset settings - wipe credentials for testing
  // gWifiManager.resetSettings();
  
  gWifiManager.setDebugOutput(debug_wifi);
  gWifiManager.setConfigPortalBlocking(true);

  // Change Mac address
  // uint8_t New_MAC_Address[] = {0x10, 0xAA, 0xBB, 0xCC, 0x33, 0xF3};
  // Serial.print("\nOLD ESP32 MAC Address: ");
  // Serial.println(WiFi.macAddress());
  // esp_wifi_set_mac(WIFI_IF_STA, New_MAC_Address);
  // Serial.print("NEW ESP32 MAC Address: ");
  // Serial.println(WiFi.macAddress());
  
  gWifiManager.setHostname(gHostName);

  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
  if(gWifiManager.autoConnect(gHostName)){
    if (debug_wifi) Serial.println("Wi-Fi connected using saved credentials");
    gFirstBoot = false;
  }
  else {
    if (debug_wifi) Serial.println("Wi-Fi manager portal running");
  }
}

