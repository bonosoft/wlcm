/*
 Blink class - non-blocking.
*/


#include "statusblink.h"

void StatusBlink::setup() {
  pinMode(LED_BUILTIN, OUTPUT);
} 

void StatusBlink::blink() {
  onMiliseconds += activityMillis;
  multiBlink = Normal;
}

void StatusBlink::loopUpdate() {
  unsigned long currentMillis = millis();
  if (ledState == LOW) {
    if (onMiliseconds > 0) {
      previousMillis = currentMillis;
      ledState = HIGH;
      if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
    } else {
      if (multiBlink>1) {
        if (currentMillis - previousMillis >= blinkLengthMillis) {
         previousMillis = currentMillis;
         ledState = HIGH;
         onMiliseconds=blinkLengthMillis;
         if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
         multiBlink--;
        }
      }
      if (currentMillis - previousMillis >= timeoutMiliseconds) {
         previousMillis = currentMillis;
         ledState = HIGH;
         onMiliseconds=blinkLengthMillis;
         if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
         if (statusMultiBlinks>1) multiBlink = statusMultiBlinks;
      }
    }
  } else {
    if (currentMillis - previousMillis >= onMiliseconds) {
      previousMillis = currentMillis;
      ledState = LOW;
      onMiliseconds=0;
      if (!isLEDportInUse) digitalWrite(LED_BUILTIN, ledState);
    }
  }
}
