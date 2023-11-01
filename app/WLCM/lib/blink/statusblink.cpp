/*
 Blink class - non-blocking.
*/


#include "statusblink.h"

StatusBlink statusBlink;

StatusBlink& StatusBlink::setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  return *this;
} 

void StatusBlink::blink() {
  onMiliseconds += activityMillis;
  multiBlink = Normal;
}

void StatusBlink::loop() {
  unsigned long currentMillis = millis();
  if (ledState == LED_OFF) {
    if (onMiliseconds > 0) {
      previousMillis = currentMillis;
      ledState = LED_ON;
      if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
    } else {
      if (multiBlink>1) {
        if (currentMillis - previousMillis >= blinkLengthMillis) {
         previousMillis = currentMillis;
         ledState = LED_ON;
         onMiliseconds=blinkLengthMillis;
         if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
         multiBlink--;
        }
      }
      if (currentMillis - previousMillis >= timeoutMiliseconds) {
         previousMillis = currentMillis;
         ledState = LED_ON;
         onMiliseconds=blinkLengthMillis;
         if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
         if (statusMultiBlinks>1) multiBlink = statusMultiBlinks;
      }
    }
  } else {
    if (currentMillis - previousMillis >= onMiliseconds) {
      previousMillis = currentMillis;
      ledState = LED_OFF;
      onMiliseconds=0;
      if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
    }
  }
}
