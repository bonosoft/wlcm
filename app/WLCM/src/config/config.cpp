#include "config.h"
#include <Arduino.h>

Config config;

void Config::factoryReset() {
    // clear config

    ESP.restart();
}

void Config::reset() {
    ESP.restart();
}

void Config::setAdminPass(char pass[]) {

}

void Config::setWifi(String ssid, String pass) {
    
}