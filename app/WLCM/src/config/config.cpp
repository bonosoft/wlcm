#include "config.h"
#include "ArduinoJson.h"
#include "LittleFS.h"
#include <Arduino.h>

Config config;

const int SETTINGS_SIZE = 8192;
StaticJsonDocument<SETTINGS_SIZE> configDoc;

const char *fileName = "/config.json";

Config& Config::append(String str) {
    if (showLoggings) Serial.print(str);
    return *this;
}


void Config::init() {
    readConfigFile();
}

void Config::initialize() {
    configDoc.clear();
}

void Config::revert() {
    initialize();
    readConfigFile();
}

void Config::save() {
    writeConfigFile();
}

void Config::readConfigFile() {
    if (LittleFS.begin()) {
        if (LittleFS.exists(fileName)) {
            File configFile = LittleFS.open(fileName, "r");
            if (!configFile) {
                log("  Failed to read configuration");
            } else {
                log("  Success reading configuration");

                deserializeJson(configDoc, configFile);
                configFile.close();
            }
        } else {
            log("  No configuration, setup needed");
        }
    }
};

void Config::writeConfigFile() {
    if (LittleFS.begin()) {
        File configFile = LittleFS.open(fileName, "w");

        if (configFile) {
            serializeJson(configDoc, configFile);
            configFile.close();

            log("  Saving config file OK");
        } else {
            log("  Saving config file failed");
        }
    }
};

void Config::factoryReset() {
    // clear config
    LittleFS.format();
    ESP.restart();
}

void Config::reset() {
    ESP.restart();
}

void Config::setAdminPass(String pass) {
    configDoc["adminpassword"] = pass;
    writeConfigFile();
}

void Config::setWifi(String ssid, String pass) {
    configDoc["wifissid"] = ssid;
    configDoc["wifipassword"] = pass;
    writeConfigFile();
}

bool Config::hasWifiSetting() {
    String wifiSsid = configDoc["wifissid"] | "";
    return wifiSsid.length()>0;
}

String Config::getSsid() {
    return configDoc["wifissid"] | "";
}

String Config::getWebPass() {
    return configDoc["wifipassword"] | "";
}
