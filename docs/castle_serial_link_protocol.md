# Castle Serial Link Communication Protocol

Castle Creations, Inc.
Version 1.5 (23-Dec-2015)

## Overview

The Serial Link device allows communication with Castle ESCs through the Castle Link Live protocol, providing real-time telemetry feedback. The device supports TTL Serial, I2C, and SPI protocols for throttle control and telemetry reading.

For more info:
- Castle Link Live Protocol: www.castlecreations.com/CastleLinkLive
- Latest docs: www.castlecreations.com/CastleSerialLink

## Pin Definitions

| Connection | TTL Serial | SPI | I2C | Analog | PPM |
|------------|------------|-----|-----|--------|-----|
| RX Port -GND | Controller ground |
| RX Port +BUS | Power from controller's internal BEC |
| RX Port Signal | Controller's signal line |
| Connector +BUS | Unregulated Power Bus |
| Connector +5V | Regulated +5.0V supply |
| Connector A | TX | CLK | SDA | Analog I/O A | |
| Connector B | RX | MISO | SCL | Analog I/O B | |
| Connector C | n/a | MOSI | n/a | Analog I/O C | |
| Connector D | n/a | NSS | n/a | Analog I/O D* | PPM Input |
| Connector -GND | Ground |

\* Analog Channel D recommended (includes filter capacitor)

## Powering the Serial Link

The Serial Link needs power through +BUS. ESCs with internal BECs power it automatically. External supply needed for HV ESCs without BEC.

## Configuration

Use a Castle Link USB device to change Serial Link settings. Connect Serial Link directly to Castle Link with included wire adapter.

## Communication Specs

### Output Modes

**Castle Link Live Protocol:**
- Real Time Telemetry Feedback
- 1.0ms to 2.0ms throttle signal
- 100Hz throttle refresh rate

**PPM (Hobby Signal):**
- 1.0ms to 2.0ms throttle signal
- 50, 100, 200, or 400Hz refresh rate

### Input Modes

**TTL Serial:**
- Device ID: 0 to 63
- Baud Rate: 1200 to 230400

**I2C:**
- 7-bit Slave Address: 8 to 71
- Frequency: 10kHz to 400kHz

**SPI:**
- Device ID: 0 to 63
- Frequency: 125kHz to 500kHz

**Analog:**
- Port: A, B, C, or D
- Ranges: Normal, Inverted, Lower Half, Upper Half, etc.

### Pass-Through Modes

- TTL Serial (with Analog Input)
- I2C (with Analog Input)
- TTL Serial (with PPM Input)
- I2C (with PPM Input)

Pass-through modes allow throttle control via receiver/potentiometer while reading telemetry.

## Registers

### Read Registers

| Addr | Name | Description |
|------|------|-------------|
| 0 | Voltage | Controller's input voltage |
| 1 | Ripple | Input voltage ripple |
| 2 | Current | Current draw |
| 3 | Throttle | Commanded throttle value |
| 4 | Power | Output throttle percentage |
| 5 | Speed | Motor electrical RPM |
| 6 | Temp | Controller temperature |
| 7 | BEC Volt | BEC voltage |
| 8 | BEC Current | BEC current load |
| 9 | Raw NTC | Raw NTC temperature value |
| 10 | Raw Linear | Raw linear temperature value |
| 25 | Link Live | Whether in Link Live mode |
| 26 | Fail Safe | E.Stop/RX Glitch fail safe output (0=1ms; 100=2ms) |
| 27 | E. Stop | If '1' output is set to fail safe |
| 28 | Packet In | Packets received |
| 29 | Packet Out | Packets sent |
| 30 | Check Bad | Packets with invalid checksums |
| 31 | Packet Bad | Packets with invalid data |

### Write Registers

| Addr | Name | Description | Range |
|------|------|-------------|-------|
| 128 | Throttle | Commanded throttle value | 0 to 65535 |
| 129 | Fail Safe | E.Stop fail safe output | 0 to 100 |
| 130 | E. Stop | Set to fail safe output | 0 or 1 |
| 131 | Packet In | Reset counter | Sets to 0 |
| 132 | Packet Out | Reset counter | Sets to 0 |
| 133 | Check Bad | Reset counter | Sets to 0 |
| 134 | Packet Bad | Reset counter | Sets to 0 |

## Throttle Register

Writing 0 [0x0000] = 1.0ms pulse (OFF)
Writing 65535 [0xFFFF] = 2.0ms pulse (FULL THROTTLE)

Fixed Throttle mode recommended for consistent response. Governor modes also supported.

## Safety Features

### Emergency Stop

E.Stop register set to '1' ignores throttle and outputs Fail Safe value. Fail Safe range: 0-100 (0=1.0ms, 100=2.0ms). Default: 1.0ms. Set to 50 (1.5ms) for reversible/car ESCs. Red LED indicates active.

### Throttle Glitch Detection

PPM signal lost >1 second triggers throttle glitch state, outputs Fail Safe value. Red LED indicates active.

### Communication Watchdog Timer

No packets received within programmable timeout (1-255 seconds) triggers Fail Safe. Set to 0 to disable.

## Conversion Factors

| Data Item | Scale | Units | Max |
|-----------|-------|-------|-----|
| Voltage | 20.0 | Volts | 100 |
| Ripple Voltage | 4.0 | Volts | 20 |
| Current | 50.0 | Amps | 250 |
| Throttle | 1.0 | Milliseconds | 2.5 |
| Output Power | 0.2502 | Percent | 1 |
| RPM | 20,416.66 | Electrical RPM | 100,000 |
| BEC Voltage | 4.0 | Volts | 20 |
| BEC Current | 4.0 | Amps | 20 |
| Temperature | 30.0 | Degrees C | 150 |
| Raw NTC Temp | 63.8125 | Units | 255 |
| Raw Linear Temp | 30.0 | Degrees C | 150 |

**Conversion formula:**
```
Result = Register / 2042 * Scale
```

Example: Voltage register = 4084
```
Voltage = 4084 / 2042 * 20 = 40.0V
```

## I2C Protocol

Commands: 5 bytes
Response: 3 bytes

### Command Format (Write)

| Byte | Description |
|------|-------------|
| 0 | I2C Slave Address (7-bit, 8-71) << 1 |
| 1 | Register number |
| 2 | Data high byte |
| 3 | Data low byte |
| 4 | Checksum |

### Response Format (Read)

Send slave address with read bit set, receive:

| Byte | Description |
|------|-------------|
| 0 | Data high byte |
| 1 | Data low byte |
| 2 | Checksum |

### Checksum

Modular sum:
```
Checksum = 0 - (Byte0 + Byte1 + Byte2 + Byte3)
```

Valid if sum of all bytes = 0x00 (ignoring overflow).

Error response: 0xFFFF

## TTL Serial Protocol

Same packet structure as I2C. First byte contains start bit + Device ID (0-63).

Clear command buffer by writing 5+ 0x00 bytes.

## SPI Protocol

Same packet structure. Synchronous - write 5 bytes minimum to receive 3-byte response. Use next transmission to receive previous response.
