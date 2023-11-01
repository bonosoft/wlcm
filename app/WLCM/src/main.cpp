#include <Arduino.h>

#include "ESPAsyncWebServer.h"

#include <statusblink.h>

#include "config/configmenu.h"
#include "config/configweb.h"

#include "web/webcaptiverequesthandler.h"
#include "web/webapp.h"
#include "web/styles.h"

AsyncWebServer webServer(80);


void setup() {
  statusBlink.setup().inverse();                   // Set the status blink class to init state (3x blinks)
  configMenu.setup();
  configWeb.setup();
 
  webServer.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);  //only when requested from AP

  webServer.on("/", HTTP_ANY, handleHtmlRequest);
  webServer.on("/styles.css", HTTP_GET, handleStylesRequest);

  // more web handlers...
 
  webServer.begin();

}

void loop() {
  configWeb.loop();
  configMenu.loop();
  statusBlink.loop();
}