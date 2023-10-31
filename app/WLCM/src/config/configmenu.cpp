#include "configmenu.h"
#include <statusblink.h>
#include "config.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>

void ConfigMenu::insert(char ch) {
    command[commandIndex] = ch;
    command[1 + commandIndex] = 0;
    if (commandIndex<30) commandIndex++;
}

void ConfigMenu::remove() {
    command[commandIndex] = 0;
    if (commandIndex>0) commandIndex--;
}

void ConfigMenu::addNetwork(String net) {
    if (networkCount>0) {
        for (int i = 0; i<networkCount; i++) {
            if (networks[i] == net) return;
        }
    }
    networks[networkCount] = net;
    networkCount++;
}

void ConfigMenu::handleInput() {
    switch (menu) {
        case Menu::FindMeShown:
            statusBlink.setStatus(AppStatus::Initializing);
            statusBlink.normal();
            menu = Menu::ShowMainMenu;
            break;

        case Menu::SearchWifiShown:
            if (strcmp("S", command) == 0 || strcmp("s", command) == 0) {
                menu = Menu::ShowSearchWifi;
            } else if (strcmp("Q", command) == 0 || strcmp("q", command) == 0) {
                menu = Menu::ShowMainMenu;
            } else {
                String inString = String(command);
                selectedNetwork = inString.toInt();
                if (selectedNetwork>=1 && selectedNetwork<=20 && selectedNetwork<=networkCount) {
                    selectedNetwork = selectedNetwork-1;
                    Serial.println("\nSelected SSID: " + networks[selectedNetwork]);
                    menu = Menu::ShowWifiPassword;
                } else {
                    Serial.println("\nSelect S, 0-x, or Q to show result, select Wi-Fi or returning to main menu.");
                }
            }
            break;

        case Menu::WifiPasswordShown:
            {
                Serial.println("Connecting to WiFi...");
                WiFi.begin(networks[selectedNetwork], command);
                int retries = 0;
                while ((WiFi.status() != WL_CONNECTED) && (retries < 20)) {
                    retries++;
                    delay(500);
                    Serial.print(".");
                }
                if (retries > 19) {
                    Serial.println(F("WiFi connection FAILED"));
                }
                if (WiFi.status() == WL_CONNECTED) {
                    Serial.println(F("WiFi connected!"));
                    Serial.println("IP address: ");
                    Serial.println(WiFi.localIP());

                    config.setWifi(networks[selectedNetwork], String(command));
                    Serial.println("\nNew Wi-Fi information was saved.");

                    menu = Menu::ShowMainMenu;
                } else {
                    menu = Menu::ShowSearchWifi;
                }
            }
            break;    

        case Menu::AdminPasswordShown:
            if (commandIndex<4) {
                Serial.println("\nPassword too short, your request was canceled and returning to main menu.");
            } else {
                String pass = String(command);
                if (pass.equalsIgnoreCase("password") || commandIndex<8) {
                    Serial.println("\nOh, security is not your thing...");
                } else {
                    Serial.println("\nAnalysing your password...");
                }
                bool hasUpper = false;
                bool hasLower = false;
                bool hasNumber = false;
                for(int i=0; i<commandIndex; i++) {
                    if (isUpperCase(command[i])) hasUpper = true;
                    if (isLowerCase(command[i])) hasLower = true;
                    if (isDigit(command[i])) hasNumber = true;
                }
                if (commandIndex<10) Serial.println("Sort passwords are easier to hack, longer is better...");
                if (!hasUpper) Serial.println("No Uppercase letters found, You Can Easily MAKe A BeTTer PassWord...");
                if (!hasLower) Serial.println("No Lowercase letters found, it is recommended to add a few...");
                if (!hasNumber) Serial.println("No digit (number) found, include 1 or more to make stronger passwords...");
                config.setAdminPass(command);
                Serial.println("New admin password was saved.");
            }
            menu = Menu::ShowMainMenu;
            break;

        case Menu::ResetShown:
            if (strcmp("Y", command) == 0) {
                config.reset();
            } else {
                Serial.println("\nCommand was not 'Y', canceled request and returning to main menu.");
                menu = Menu::ShowMainMenu;
            }
            break;

        case Menu::FactoryResetShown:
            if (strcmp("Y", command) == 0) {
                Serial.println("\nFactory Reset start...");
                config.factoryReset();
            } else {
                Serial.println("\nCommand was not 'Y', canceled  request and returning to main menu.");
                menu = Menu::ShowMainMenu;
            }
            break;

        case Menu::MainMenuShown:
            if (strcmp("1", command) == 0) {
                menu = Menu::ShowFindMe;
            } else if (strcmp("2", command) == 0) {
                menu = Menu::ShowSearchWifi;
            } else if (strcmp("3", command) == 0) {
                menu = Menu::ShowAdminPassword;
            } else if (strcmp("4", command) == 0) {
                menu = Menu::ShowReset;
            } else if (strcmp("9", command) == 0) {
                menu = Menu::ShowFactoryReset;
            } else if (strcmp("q", command) == 0 || strcmp("Q", command) == 0) {
                menu = Menu::None;
                config.startLogging();
            } else {
                menu = Menu::ShowMainMenu;
            }
            break;

        default:
            menu = Menu::ShowMainMenu;
            break;
    }

    command[0] = 0;
    commandIndex = 0;
}

void ConfigMenu::showMenu() {
        switch (menu) {
        case Menu::ShowMainMenu:
            config.stopLogging();
            Serial.println("\n---\n\nWLCM Node version " + (String)VERSION + " on ID-" + (String)ESP.getChipId());
            Serial.println("\n   WLCM Main Administration Menu");
            Serial.println(" 1 Find me");
            Serial.println(" 2 Search for Wi-Fi");
            Serial.println(" 3 Set Web Admin Password");
            Serial.println(" 4 Restart");
            Serial.println(" 9 Factory reset");
            Serial.println(" Q Hide menu and show logging");
            Serial.print("Select: ");
            menu = Menu::MainMenuShown;
            break;

        case Menu::ShowFindMe:
            Serial.println("\n\n-- WLCM Find me --");
            Serial.println(" Q  Stop Find Me blinks and show main menu");
            Serial.print("Select: ");
            menu = Menu::FindMeShown;
            statusBlink.setStatus(AppStatus::Findme);
            statusBlink.rapid();
            break;

        case Menu::ShowSearchWifi:
            Serial.println("\n\n-- WLCM Wi-Fi Search --");
            Serial.println(" S Show Wi-Fi Scanning (retry if a network is missing)\n");
            {
                int n = WiFi.scanComplete();
                if (n == -2) {
                    WiFi.scanNetworks(true);
                } else if(n) {
                    networkCount = 0;
                    if (n>20) n = 20;
                    for (int i = 0; i < n; ++i) {
                        addNetwork(WiFi.SSID(i));
                    }
                    WiFi.scanDelete();
                    if (WiFi.scanComplete() == -2) {
                        WiFi.scanNetworks(true);
                    }

                    if (networkCount>0) {
                        for (int i = 0; i < networkCount; ++i) {
                            Serial.println(" " + String(1+i) + " " + networks[i]);
                        }
                    }
                }
            }
            Serial.println("\n Q Return to main menu");
            Serial.print("Select: ");
            menu = Menu::SearchWifiShown;
            break;

        case Menu::ShowWifiPassword:
            Serial.println("\n-- Provide Password for " + networks[selectedNetwork] + "--");
            Serial.print("Password: ");
            menu = Menu::WifiPasswordShown;
            break;    

        case Menu::ShowAdminPassword:
            Serial.println("\n-- WLCM Provide Password for Web Administration User --");
            Serial.println("Valid passwords are between 8-30 characters, longer passwords are more secure");
            Serial.println("Strong passwords contains capital and lowercase letters, numbers AND special characters like !?");
            Serial.print("Password: ");
            menu = Menu::AdminPasswordShown;
            break;    

        case Menu::ShowReset:
            Serial.println("\n\n-- WLCM Restart the controller --");
            Serial.println(" Y  Restart controller");
            Serial.println(" Q  Return to the main menu");
            Serial.print("Select: ");
            menu = Menu::ResetShown;
            break;

        case Menu::ShowFactoryReset:
            Serial.println("\n\n-- WLCM Factory Reset --");
            Serial.println(" Y  DELETE all configuration and restart controller");
            Serial.println(" Q  No, take be back...");
            Serial.print("Select: ");
            menu = Menu::FactoryResetShown;
            break;


        default:
            break;
    }
}


void ConfigMenu::loopMenu() {
    if (Serial.available()) {
        char ch = Serial.read();
        if (ch=='\n') {
            handleInput();
        } else if (ch==8) {
            Serial.print(ch);
            remove();
        } else if (ch>=' ') {
            Serial.print(ch);
            insert(ch);
        }
    }

    showMenu();
}