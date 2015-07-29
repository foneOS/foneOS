# foneOS
A simple phone "operating system", designed to run on the Intel Edison.

Technically, this is *not* an operating system due to the fact that it runs on top of Linux (or Windows, if using the simulator), so it does not handle low-level tasks.

## Features
* Basic GUI/layout system
* TrueType font support
* Basic settings page
* Basic dialer
* Simulator 
* (soon) App support (using LuaJIT)

### Sidenote
It may seem like there are lots of applications missing. This is because lots of the planned internal apps (SMS, FM Radio, etc.) are all waiting on the app system to be working before they are developed.

## Hardware
Currently, FoneOS can only be run in a simulator powered by SDL.

However, work is being done to support it on an actual device with the following hardware:
* [Intel Edison (with Arduino breakout)](https://www.sparkfun.com/products/13097)
* [Adafruit FONA 800](http://www.adafruit.com/product/1946)
* [SeeedStudio 2.8" TFT Touch Shield v1](http://www.seeedstudio.com/depot/28-tft-touch-shield-p-864.html)

Note that support for another touch shield may be added in the future because the current one has been discontinued by Seeed.

## Dependencies
### For all builds
* freetype2
* SQLite
* LuaJIT

#### Already included (no additional download required)
* pugixml

### For simulator builds
* SDL2

