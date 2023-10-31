#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

const char VERSION[] = "0.1";

class Config {

    bool showLoggings = true;

public:
    void log(int i) {
        if (showLoggings) Serial.println(i);
    }

    void log(char* str) {
        if (showLoggings) Serial.println(str);
    }

    void log(String str) {
        if (showLoggings) Serial.println(str);
    }


    void startLogging() {
        showLoggings = true;
    }

    void stopLogging() {
        showLoggings = false;
    }

    void reset();
    void factoryReset();

    void setAdminPass(char pass[]);
    void setWifi(String ssid, String pass);

};

extern Config config;

#endif