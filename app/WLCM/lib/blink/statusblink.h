/*
 LCCM Status blinking - non-blocking.
 Author Bo Norgaard
 Copyright by Bonosoft, all rights reserved
*/

#ifndef STATUSBLINK_H
#define STATUSBLINK_H

#include "Arduino.h"

enum AppStatus { Normal = 1, Warning = 2, Initializing = 3, Error = 4, Closing = 5, Findme = 9 };

class StatusBlink {

  char LED_ON = HIGH;
  char LED_OFF = LOW;

  int ledState = LED_OFF;                   // current state of the LED
  unsigned long blinkLengthMillis = 200;    // ON miliseconds of a status blink
  int activityMillis = 500;                 // ON miliseconds of an activity blink

  unsigned long previousMillis = 0;         // last loop update time in miliseconds
  unsigned long onMiliseconds = 0;          // ON miliseconds to keep the LED on this blink
  int multiBlink = 0;                       // number of status blinks for current status
  
  unsigned long timeoutMiliseconds = 10000;          // OFF miliseconds between status blinks
  int statusMultiBlinks = Initializing;     // Number of blinks for current application status (3 default for initialization)

  bool isLEDportInUse = false;              // Build-in LED pin is used for other use
             

  public:

    // constructor
    StatusBlink() {}

    // destructor - frees memory used
    ~StatusBlink() { }

    // handle initialization of the pin
    void setup();

    void blink();

    // handle incoming data, return true if packet ready
    void loopUpdate();

    void setStatus(AppStatus appStatus) {
      statusMultiBlinks = appStatus;
    }

    void inverse() {
      LED_ON = LOW;
      LED_OFF = HIGH;
    }

    // When the pin with the build in LED is used to other use, don't set the pin
    void setBuildInLedPinInUse(bool inUse) {
      isLEDportInUse = inUse;
    }

    // When the LED pin is in use for other use, read the virtuel blink status
    bool virtualBlinkState() {
      return ledState == HIGH;
    }

    void rapid() {
        blinkLengthMillis = 100;
        timeoutMiliseconds = 3000;
    }

    void normal() {
        blinkLengthMillis = 200;
        timeoutMiliseconds = 10000;
    }

};

extern StatusBlink statusBlink;

#endif 
