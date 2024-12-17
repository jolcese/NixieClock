#include <WebServer.h>
#include <nixies.h>

WebServer gWebServer(80);

void setupApis() {
  gWebServer.on("/nixie", HTTP_POST, []() {
    Serial.println("/nixie(POST)  ");
    Serial.print("on = ");
    Serial.println(gWebServer.arg("on"));
    switchNixies((bool) gWebServer.arg("on").toInt());

    // Serial.println(gWebServer.arg("mode"));
    // switchNixies((bool) gWebServer.arg("on").toInt());

    gWebServer.send(200, "text/plain", "");
    gWebServer.sendHeader("Access-Control-Allow-Origin", "*");
  });
  
  gWebServer.begin();
}

void handleApis() {
    gWebServer.handleClient();
}