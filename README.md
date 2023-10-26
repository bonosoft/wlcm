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

## Wi-Fi Micro Controller
WLCM uses an ESP8266 controller from the company Espressif. It is a very cost-effective micro controller integrated to a single unit and includes support for Wi-Fi. You can use an ESP32, but unless you need a huge number of connected items, it will sit idle most of the time and it is more expensive than the ESP8266.

WLCM supports most ESP8266 boards that contains a USB connector. The USB connector is used to deploy the WLCM application, to monitor the generated log information, and to perform a password reset of the application. Password reset function is by design (security) only available via the USB serial connection, requiring a physical connection to the controller. This is especially important if you bring your modules to a club or a model train event, with public access.

An additional bonus of the USB connection is that these boards contain a 3.3V power regulator for the micro controller. This ensures that you only need to supply 5V power to operate the controller and optional extension modules. 

## Micro controller module
The “Wemos ESP8266 LOLIN D1 mini” board is used in this guide. The larger but still very affordable “Wemos ESP32 S2 mini” with double as many pins is also supported and tested with WLCM.

![image](https://github.com/bonosoft/wlcm/assets/5933560/71906338-d087-4d45-8ec4-085aabf6cb65)

If you use another ESP8266 module, the number of available pins and the physical pin numbers might be different. Do check the Datasheet to find the mapping of the ESP8266 port numbers to the external pin numbers on the controller. You need to find the pin numbers for I2C (SCL, SDA) and SPI (SCK, MISO, MOSI) and change the standard configuration for these in WCLM web administration. Because the pins are configurable, almost all ESP8266 and ESP32 boards will work with WCLM.

## Minimal Requirements
All you need to get started is one ESP8266 module board and a USB cable. Wemos D1 mini V4.0.0 board with USB-C is available for around $2. The board contains 8 digital PWM pins (3 more if you want to use the shared LED, RX, TX pins), and a pin with an ADC for measuring voltage.

When connecting items directly to the controller, you need to be aware of the maximum power you can draw from each pin, and from all the pins in total. The controller pins are operating at 3.3V.

## Extension Modules
If the number of pins on the micro controller are not enough for connecting all your items, you can expand with one or more extension boards:
•	Up to 8 boards with 16 I/O pins, total of 128 pins (I2C)
•	Up to 62 board with 16 PWM Servo pins, total of 992 servos (I2C)
•	“Unlimited” PWM LED modules with 24 pins each, total of all you need. (SPI)
If you need both IC2 (2 pins) and SPI (3 pins) extension modules, you still have 3 PWM digital usable pins directly on the controller.

If the above does not meet your requirement, you can use a Wemos S2 mini (ESP32) instead, which have 27 available pins, supporting four SPI and two I2C.

The controller boards are very affordable, so we do recommend separating the installations and using only a small amount of extension boards on each. The basic idea for WLCM was after all to limit use of long and complicated wiring. Building your layout with small separated manageable blocks of controllers and items, is recommended and will help you a lot.

Yes, you can control a full large layout with a single WLCM controller and a lot of extension boards, but that does not mean you should. 

# Input and Output types
The pins on the controller and expansion modules can be used in different ways.

## Standard Input
The standard I/O pins can have two logical values, HIGH and LOW (on and off)
Standard Input pins are normally used for push buttons and contacts, allowing user interactions and to check turnouts position. You can also connect Infra-Red (IR) circuits and current detectors to check for track occupancy. Some pins do need an external pull-up resistor to shift from low to high, as the controller is only able to pull the pin low and expect it to go high when it’s not forcing it down.

## Standard Output
The standard I/O pins can have two logical values, HIGH and LOW (on and off)
Standard output is used for switching items on and off, which can be lighting or other circuits. If you need to control items that needs more power than the output pin can provide, the pin can control power transistors or relays.

## PWM LED Output
A light emitting diodes pins allow you to control a LED with 4096 different levels. This lets you control the amount of lights it emits, so you can ease in or fade out lights, normally used in signals and emulating daylight. The extension module uses current limitation, which allows you to add one or more LEDs directly to the pin without a resistor.

## PWM Servo Output
These output pins are specially made for controlling Servo motors. Servos are typically used for switches but can be used to make things move in all parts of your layout, like crossings.
