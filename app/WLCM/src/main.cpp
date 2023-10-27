#include <Arduino.h>
#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"
#include "web/webcaptiverequesthandler.h"
#include "utils/statusblink.h"

DNSServer dnsServer;
AsyncWebServer server(80);
StatusBlink statusBlink; 

// ResetHandler resetHandler;


void setup(){
  Serial.begin(115200);
  delay(500);
  Serial.print("WLCM Node version 1.0 on ID-" + (String)ESP.getChipId());

  // Connect to Wi-Fi

  // if Wi-Fi connection failed, start AP
  WiFi.softAP("WLCM Node " + (String)ESP.getChipId());
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP

  // more web handlers...
 
  server.begin();
  statusBlink.setup();                   // Set the status blink class to init state (3x blinks)

}

void loop(){
  dnsServer.processNextRequest();
  // resetHandler.processRequests();
  statusBlink.update(false);
}