# Phase Control

Idea: Extend the [Phase control board](https://www.amazon.de/dp/B07SFF9VC6?ref_=cm_sw_r_apan_dp_5E5BYS5TRJ6GNMP8MNYR&language=de-DE) with an EPS32 so it can be controlled via a webinterface and add an api for homeautomation.

![Build](docs/build.jpg)

## How it works
If no wifi is configured, the ESP opens a Wifi with `ESPUI-<chipId>`. Connect to the wifi and open the webinterface at [http://192.168.0.1](http://192.168.0.1). If you want to change the wifi:
- Tab: Wifi Setting
- Enter your ssid and passwort
- Click "Save and Restart"

On successfull startup, the ESP enters `recalibration mode`, which means it is pressing the button down button for 15 sec.

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

### Stuff

- **Roadmap:** [Link to Roadmap](docs/roadmap.md)
- **Hardware Notes:** [Link to Hardware Notes](docs/hardware.md)