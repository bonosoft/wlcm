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

#include <statusblink.h>
#include "config/configmenu.h"
#include "web/webcaptiverequesthandler.h"
#include "web/webapp.h"
#include "web/styles.h"

DNSServer dnsServer;
AsyncWebServer webServer(80);
ConfigMenu configMenu;
StatusBlink statusBlink;
// ResetHandler resetHandler;


void setup() {
  statusBlink.setup();                   // Set the status blink class to init state (3x blinks)
  statusBlink.inverse();
  configMenu.setup();
  

  // Connect to Wi-Fi

  // if Wi-Fi connection failed, start AP
  WiFi.softAP("WLCM Node " + (String)ESP.getChipId());
  dnsServer.start(53, "*", WiFi.softAPIP());
  webServer.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
  
  webServer.on("/", HTTP_ANY, handleHtmlRequest);
  webServer.on("/styles.css", HTTP_GET, handleStylesRequest);

  // more web handlers...
 
  webServer.begin();

}

void loop() {
  configMenu.loopMenu();
  dnsServer.processNextRequest();
  // resetHandler.processRequests();
  statusBlink.loopUpdate();
}