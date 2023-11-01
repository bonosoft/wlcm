#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "ArduinoJson.h"

const char VERSION[] = "0.2";

class Config {

    bool showLoggings = true;

    void initialize();
    void revert();
    void save();
    void readConfigFile();
    void writeConfigFile();

public:
    void init();

    void log(int i) {
        if (showLoggings) Serial.println(i);
    }

    void log(String str) {
        if (showLoggings) Serial.println(str);
    }

    void log(byte data[], int length) {
        if (showLoggings) {
            for(int i=0; i<(length-1); i++) {
                Serial.print(data[i] < 16 ? "0" : "");
                Serial.print(data[i], HEX);
                Serial.print(":");
            }
            Serial.println(data[length-1], HEX);
        }
    }

    Config& append(String str);

    void newline() {
        if (showLoggings) Serial.println();
    }

    void startLogging() {
        showLoggings = true;
    }

    void stopLogging() {
        showLoggings = false;
    }

    void reset();
    void factoryReset();

    void setAdminPass(String pass);
    
    bool hasWifiSetting();
    void setWifi(String ssid, String pass);
    String getSsid();
    String getWebPass();


};

extern Config config;

#endif