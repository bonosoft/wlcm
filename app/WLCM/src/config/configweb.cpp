#include <Arduino.h>
#include <DNSServer.h>

#ifdef ESP32
#include <AsyncTCP.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include "config.h"
#include "configweb.h"
#include <statusblink.h>

ConfigWeb configWeb;
DNSServer dnsServer;

void ConfigWeb::setup() {
        config.log("\n");
        config.log("*************************************");
        config.log("");
        config.log("    W E L C O M E   T O   W L C M  ");
        config.log("");
        config.log("  Press 'ENTER' to show main menu. ");
        config.log("");

    WiFi.mode(WIFI_AP_STA);
    if (config.hasWifiSetting()) {
        config.log("  Wifi   : " + config.getSsid());
        connectWifi();
    }
    startWifiAp();
    config.log("*************************************");
}

void ConfigWeb::startWifiAp() {
    WiFi.softAP("WLCM Node " + (String)ESP.getChipId());
    dnsServer.start(53, "*", WiFi.softAPIP());
    config.log("  AP name: WLCM Node " + (String)ESP.getChipId());
    WiFi.softAPmacAddress(mac);
    config.append("  AP MAC : ").log(mac, sizeof(mac));
    openAccessPoint = true;
    statusBlink.setStatus(AppStatus::Error);
}

void ConfigWeb::stopWifiAp() {
    dnsServer.stop();
    WiFi.softAPdisconnect(true);
    openAccessPoint = false;
    config.log("  DNS Server stopped.");
    config.log("  Wifi Access Point stopped.");
}

void ConfigWeb::connectWifi() {
    if (config.hasWifiSetting()) {
        WiFi.begin(config.getSsid(), config.getWebPass());
        WiFi.setAutoReconnect(true);
        WiFi.persistent(true);
    }
}

void ConfigWeb::loop() {
    dnsServer.processNextRequest();

    if (config.hasWifiSetting()) {
        unsigned long currentMillis = millis();
        if (currentMillis - wifiLastLoopCheckTime >= wifiLoopInterval) {
            switch (WiFi.status()) {

                case WL_NO_SSID_AVAIL:
                    config.log("Wi-Fi: Configured SSID cannot be reached");
                    wifiClientConnectedCount = 0;
                    if (!openAccessPoint) {
                        config.log("Wi-Fi configuration issue, starting Access Point...");
                        startWifiAp();
                    }
                    break;

                case WL_IDLE_STATUS:
                    config.append("."); // Wifi is trying to connect.
                    break;

                case WL_CONNECTED:
                    if (wifiClientConnectedCount == 0) {
                        WiFi.macAddress(mac);
                        config.log("  NET: " + config.getSsid());
                        config.append("  IP : ").log(WiFi.localIP().toString());
                        config.append("  MAC: ").log(mac, sizeof(mac));
                        config.log("");
                        statusBlink.setStatus(AppStatus::Warning);
                    }
                    wifiClientConnectedCount++;
                    if (wifiClientConnectedCount>10) {
                        if (openAccessPoint) {
                            // wifi_softap_get_station_num(void)
                            config.log("*************************************");
                            config.log("Wi-Fi connected, closing Access Point");
                            stopWifiAp();
                            config.log("");
                        }
                        wifiClientConnectedCount = 1;
                    }
                    break;

                case WL_CONNECTION_LOST:
                    config.log("Wi-Fi: Connection lost.");
                    break;

                case WL_DISCONNECTED:
                    config.log("Wi-Fi: Connection disconnected");
                    break;

                case WL_CONNECT_FAILED:
                    config.log("Wi-Fi: Connection failed");
                    wifiClientErrorCount++;
                    if (wifiClientErrorCount > 10) {
                        config.log("Wifi has to many errors, force disconnect...");
                        WiFi.disconnect();
                        wifiClientErrorCount = 0;
                        connectWifi();
                        config.log("Wifi trying to connect client to network...");

                        wifiClientConnectedCount = 0;
                        if (!openAccessPoint) {
                            config.log("Wi-Fi might have a configuration issue, starting Access Point...");
                            startWifiAp();
                        }
                    }
                    break;

                case WL_WRONG_PASSWORD:
                    config.log("Wifi password was not accepted for network " + config.getSsid());
                    break;

                case WL_SCAN_COMPLETED:
                    config.log("Wifi scanning for networks completed.");
                    break;

                default:
                    config.log("Wifi status is unknown.");
                    break;
            }

            if (WiFi.status() == WL_CONNECTED) {
                config.append("  NET: " + config.getSsid());
                config.append(", IP: ").append(WiFi.localIP().toString());
                config.append(", RRSI: ").log(WiFi.RSSI());
            }

            wifiLastLoopCheckTime = millis(); // disconnect and connect can take a long time, so lets use current time...
        }
    }
}
