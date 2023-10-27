#ifndef WEB_CAPTIVE_REQUEST_HANDLER_H
#define WEB_CAPTIVE_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request);
};

#endif