# foneOS [![Build Status](https://img.shields.io/wercker/ci/foneOS/foneOS.svg)](https://app.wercker.com/foneOS/foneOS)

A simple phone "operating system", designed to run on the Intel Edison.

Technically, this is *not* an operating system due to the fact that it runs on top of Linux (or your desktop OS, if using the simulator), so it does not handle low-level tasks.

## Features
* Basic GUI/layout system
* TrueType font support
* Basic settings page
* Basic dialer
* Simulator 
* App support (using LuaJIT)

### Note on apps
It may seem like there are lots of applications missing. This is because lots of the planned internal apps (SMS, FM Radio, etc.) are all waiting on the app system to be working before they are developed.

## Hardware
FoneOS currently supports the following hardware:
### On computer
* A simulated touch screen (240x320, powered by SDL)
* A simulated modem that can't really do anything

### On [Intel Edison (with Arduino breakout)](https://www.sparkfun.com/products/13097)
#### Modems
* [Adafruit FONA 800 (SIM800)](http://www.adafruit.com/product/1946)

#### Displays
* [SeeedStudio 2.8" TFT Touch Shield v1 (ST8891R)](http://www.seeedstudio.com/depot/28-tft-touch-shield-p-864.html)
* [Adafruit 2.8" TFT Touch Shield for Arduino with Capacitive Touch Screen (ILI9341)](https://www.adafruit.com/products/1947)

#### Touch screens
* [SeeedStudio 2.8" TFT Touch Shield v1 (4-wire generic resistive)](http://www.seeedstudio.com/depot/28-tft-touch-shield-p-864.html)
* [Adafruit 2.8" TFT Touch Shield for Arduino with Capacitive Touch Screen (FT6206)](https://www.adafruit.com/products/1947)

Note that while foneOS has been tested working on the Intel Edison, it should technically probably maybe work on anything that supports MRAA, assuming you wire everything correctly.

## Dependencies
### For all builds
* freetype2
* SQLite
* LuaJIT

#### Already included (no additional download required)
* pugixml

### For simulator builds
* SDL2

### For production builds
* [MRAA](https://github.com/intel-iot-devkit/mraa)
