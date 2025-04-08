#include <WebServer.h>
#include <nixies.h>
#include <leds.h>
#include <storage.h>

WebServer gWebServer(80);

void setupApis() {
  gWebServer.on("/config", HTTP_POST, []() {
    Serial.println("/config(POST)  ");

    if (gWebServer.hasArg("nixieMode")) {
      Serial.print("nixieMode = ");
      Serial.println(gWebServer.arg("nixieMode"));
      setNixiesMode(gWebServer.arg("nixieMode").toInt());
      storageConfigWrite();
    }
 
    if (gWebServer.hasArg("ledMode")) {
      Serial.print("ledMode = ");
      Serial.println(gWebServer.arg("ledMode"));
      setLedsMode(gWebServer.arg("ledMode").toInt());
      storageConfigWrite();
    }

    gWebServer.send(200, "text/plain", "");
    gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
  });
  
  gWebServer.on("/reboot", HTTP_POST, []() {
    gWebServer.send(200, "text/plain", "");
    gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
    ESP.restart();
  });

  gWebServer.begin();
}

void handleApis() {
    gWebServer.handleClient();
}