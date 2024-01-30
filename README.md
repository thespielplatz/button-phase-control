# Phase Control

Extend the XXXX Phase control board with an EPS32 so it can be controlled via a webinterface and add an api for homeautomation.

- [x] Prototype Build on Board
- [x] add current Value
- [x] add inital reset & reset button
- [x] Show Reset time in status label
- [x] Disable UI During reset
- [x] Refactor UI Controls of Phase Control into class
- [x] convert to platform IO
- [x] add github pilot
- [x] Change Wifi settings incl. addESPUITab
- [ ] Power supply 
- [x] Build on Board
- [ ] Auto Reset on startup
- [ ] add api?

### Hardware configuration
- IO33 for Green / Down
- TMS for Yellow / Up
- 2n2222 NPN Transistors with an 2k2 Ohm Resistor
- GND connected on both
- ESP32 with external Power Supply, because the board could only deliver had 6mA

#### Notes
- BJT: https://www.diyengineers.com/2020/12/17/2n2222-transistor-npn/
- ESP32 Pin Max Current: Each GPIO pin is rated at 40mA for a ESP32. It is recommended that a max of 20mA per GPIO pin for best thingies

### Plattform.io Known Errors

#### `no matching function for call to 'LITTLEFSImpl::open(const char*&, const cha...`
- https://github.com/lorol/LITTLEFS/issues/43

#### `Undefined reference to SHA1Init`
- https://github.com/me-no-dev/ESPAsyncWebServer/issues/1151
