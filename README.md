# foneOS
A simple phone "operating system", designed to run on the Intel Edison.

Technically, this is *not* an operating system due to the fact that it runs on top of Linux (or Windows, if using the simulator), so it does not handle low-level tasks.

## Features
* Basic GUI/layout system
* TrueType font support
* Basic settings page
* Basic dialer
* Simulator 
* (soon) App support (using Mono)

### Sidenote
It may seem like there are lots of applications missing. This is because lots of the planned internal apps (SMS, FM Radio, etc.) are all waiting on the app system to be working before they are developed.

## Dependencies
### For all builds
* freetype2
* SQLite

#### Already included (no additional download required)
* pugixml

### For simulator builds
* SDL2

