#include "webapp.h"

const char css[] PROGMEM = "";

const char index_html[] PROGMEM = "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"stylesheet\" href=\"styles.css\"><title>Test Index</title></head><body>Hello World</body></html>";


void handleHtmlRequest(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
}

