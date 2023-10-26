# WLCM
Wireless model train Layout Control for Modules

An easy to use and heavily extendable implementation of a train layout control system with a Message Queue (MQTT) for modules. Building and running model railroads in separate compatible modules has its pros and cons. The module standards (Fremo, Free-mo, NTrack, T-Trak) all defines the wiring and command control of driving and controlling trains. The only definition about controlling everything else (Turnouts, signals, lightning, track occupancy…) is that it must be completely separated from train controlling system. WLCM will help you control your modules, both at home, in your club and at events running with other module owners.

## Overview
WLCM is the easy and simple way to control a large model train layout, and especially a large layout made by modules. Wiring up all the layout elements: lightning, signals, turnouts, crossings, etc. is a huge task, and the possibility for errors during installation and maintenance over time is a serious challenge for most. WLCM enable you to make small separate installations independent of each other, and all connected with a standard Wi-Fi wireless network.

## WLCM Components
The key component is a Wi-Fi enabled micro controller. This is all you need if your module or layout section only has a few items to control. To control more items, you can connect one or more interface modules to the controller, like:
•	Module with 16 Input or Output ports
•	Module with 16 Servo motor ports
•	Modules with 24 dimmable diode light ports

![image](https://github.com/bonosoft/wlcm/assets/5933560/9a97d1b9-176e-47f2-a07e-aaab8e896f26)

The micro controller communicates over Wi-Fi with your management application (for example JMRI) through a message queue (MQTT).

# Wi-Fi Micro Controller
WLCM uses an ESP8266 controller from the company Espressif. It is a very cost-effective micro controller integrated to a single unit and includes support for Wi-Fi. You can use an ESP32, but unless you need a huge number of connected items, it will sit idle most of the time and it is more expensive than the ESP8266.

WLCM supports most ESP8266 boards that contains a USB connector. The USB connector is used to deploy the WLCM application, to monitor the generated log information, and to perform a password reset of the application. Password reset function is by design (security) only available via the USB serial connection, requiring a physical connection to the controller. This is especially important if you bring your modules to a club or a model train event, with public access.

An additional bonus of the USB connection is that these boards contain a 3.3V power regulator for the micro controller. This ensures that you only need to supply 5V power to operate the controller and optional extension modules. 

