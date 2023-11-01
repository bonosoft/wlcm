#ifndef CONFIG_WEB_H
#define CONFIG_WEB_H

class ConfigWeb {

    bool openAccessPoint;

    unsigned long wifiLastLoopCheckTime = 0;
    unsigned long wifiLoopInterval = 30000; // 30 seconds loop updates

    unsigned long wifiClientErrorCount = 0;
    unsigned long wifiClientConnectedCount = 0;

    void connectWifi();

    byte mac[WL_MAC_ADDR_LENGTH];

public:

    void setup();

    void startWifiAp();
    void stopWifiAp();

    void loop();

};

extern ConfigWeb configWeb;

#endif