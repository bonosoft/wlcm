/*
 Blink class - non-blocking.

*/


#include "statusblink.h"

// allocate the requested buffer size
void StatusBlink::setup() {
  pinMode(LED_BUILTIN, OUTPUT);
} 

void StatusBlink::blink() {
  interval += 500;
  multiBlink = 1;
}

void StatusBlink::update(bool isLEDportInUse) {
  unsigned long currentMillis = millis();
  if (ledState == LOW) {
    if (interval > 0) {
      previousMillis = currentMillis;
      ledState = HIGH;
      if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
    } else {
      if (multiBlink>1) {
        if (currentMillis - previousMillis >= 200) {
         previousMillis = currentMillis;
         ledState = HIGH;
         interval=200;
         if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
         multiBlink--;
        }
      }
      if (currentMillis - previousMillis >= timeout) {
         previousMillis = currentMillis;
         ledState = HIGH;
         interval=200;
         if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
         if (timeoutType>1) multiBlink = timeoutType;
      }
    }
  } else {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      ledState = LOW;
      interval=0;
      if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
    }
  }
}
