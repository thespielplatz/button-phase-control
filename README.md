# Phase Control

Idea: Extend the [Phase control board](https://www.amazon.de/dp/B07SFF9VC6?ref_=cm_sw_r_apan_dp_5E5BYS5TRJ6GNMP8MNYR&language=de-DE) with an EPS32 so it can be controlled via a webinterface and add an api for homeautomation.

- Currently the ESP opens a Wifi with `ESPUI-<chipId>`. Connect to the wifi and open the webinterface at [http://192.168.0.1](http://192.168.0.1). If you want to change the wifi:
  - Tab: Wifi Setting
  - Enter your ssid and passwort
  - Click "Save and Restart"
- On successfull startup the ESP enters recalibration mode, which means it is pressing the button down button for 15 sec.

## API Endpoints

### GET /api/current-value

This endpoint retrieves the current value of the phase control.

**Request:** This endpoint does not require any parameters.

**Response:**  If the request is successful, the server will respond with a `200 OK` status and an integer in the body.

### GET /api/target-value

This endpoint retrieves the target value which the phase control is moving the current value to.

**Request:** This endpoint does not require any parameters.

**Response:**  If the request is successful, the server will respond with a `200 OK` status and an integer in the body.

### POST /api/target-value

This endpoint sets the target value.

#### Request

**Request:** The request body should be a JSON object with the following properties:

- `value` (number): The target value to set.

Example:

```json
{
    "value": 42
}
```

**Response 200:**  If the request is successful, the server will respond with a `200 OK` status and an integer in the body. The return value will be the target value of the phase control.

**Response 400:**  If the parameters of the request are not correct, the server will respond with a `400 BAD REQUEST` status and an integer in the body. The return value will be `-1`.


## Roadmap

- [x] Prototype Build on Board
- [x] add current Value
- [x] add inital reset & reset button
- [x] Show Reset time in status label
- [x] Disable UI During reset
- [x] Refactor UI Controls of Phase Control into class
- [x] convert to platform IO
- [x] add github pilot
- [x] Change Wifi settings incl. addESPUITab
- [x] Power supply 
- [x] Build on Board
- [x] Auto Reset on startup
- [ ] add api

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
