#ifndef CONFIG_MENU_H
#define CONFIG_MENU_H

#include <Arduino.h>
#include "config.h"

enum Menu {
    None,
    ShowMainMenu,
    MainMenuShown,
    ShowFindMe,
    FindMeShown,
    ShowSearchWifi,
    SearchWifiShown,
    ShowWifiPassword,
    WifiPasswordShown,
    ShowAdminPassword,
    AdminPasswordShown,
    ShowReset,
    ResetShown,
    ShowFactoryReset,
    FactoryResetShown
};

class ConfigMenu {

    Menu menu = Menu::None;

    int commandIndex = 0;
    char command[32] = "";

    int networkCount = 0;
    String networks[20];
    int selectedNetwork = 0;
    
    void insert(char ch);
    void remove();
    void handleInput();
    void showMenu();
    void addNetwork(String net);

public:

    void setup() {
        Serial.begin(115200);      // Setup USB Serial connection for debug/status logging
        delay(500);
        Serial.println("\n\n-------------------------------------");
        Serial.println("  WLCM Node ver " + (String)VERSION + "   ID-" + (String)ESP.getChipId());
        Serial.println("-------------------------------------");
        config.init();
        Serial.println("-------------------------------------");
    }

    void loop();

};

extern ConfigMenu configMenu;

#endif
