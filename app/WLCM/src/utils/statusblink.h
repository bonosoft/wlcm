/*
 LCCM Status blinking - non-blocking.
 Author Bo Norgaard
 Copyright by Bonosoft, all rights reserved
*/

#ifndef STATUSBLINK_H
#define STATUSBLINK_H

#include "Arduino.h"


class StatusBlink {

  int ledState = LOW;             
  unsigned long previousMillis = 0;        
  long interval = 0;           
  long timeout = 10000;
  int timeoutType = 3;
  int multiBlink = 0;
             

  public:

    // constructor
    StatusBlink() {}

    // destructor - frees memory used
    ~StatusBlink() { }

    // handle initialization of the pin
    void setup();

    void blink();

    // handle incoming data, return true if packet ready
    void update(bool isLedInUse);

    void setTimeoutType(int type) {
      timeoutType = type;
    }

};

#endif 
