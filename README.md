# DFRobot_RTK_LG_EG
- [Chinese Version](./README_CN.md)

This is a high‑precision GNSS‑RTK positioning device designed for enterprise‑level applications, capable of achieving centimeter‑level outdoor positioning accuracy.  
The product integrates a 4G communication module and supports quick access to RTK differential services via the Ntrip protocol. It can also be connected to an external wireless data link module to achieve high‑accuracy positioning, making it easy to adapt to various deployment environments.

The device provides rich interfaces including UART and USB, which makes it easy to integrate into different systems and extend functions. It supports rapid data verification on an upper‑computer, has power‑down data retention, and can be expanded with an SD card to ensure important data is not lost.

At its core, the module uses Quectel LG290P03AAMD, supporting multi‑constellation and multi‑frequency reception with an output rate up to 20 Hz (10 Hz by default). With built‑in interference detection and suppression algorithms, it can still maintain accurate positioning even in complex electromagnetic environments or urban canyons.

This product is suitable for intelligent robot navigation, precision agriculture operations, industrial park AGV positioning robots, and other scenarios. It is an ideal choice for enterprise users to quickly validate positioning performance and integrate RTK into projects.

![Front/back view](/resources/images/TEL0193.png)

## Product Link (https://www.dfrobot.com)

    SKU: TEL0193

## Contents

* [Overview](#overview)
* [Library Installation](#library-installation)
* [APIs](#apis)
* [Compatibility](#compatibility)
* [History](#history)
* [Author](#author)

## Overview

This repository provides an Arduino library for the RTK module. The library mainly supports:

- Reading GNSS GGA data  
- Configuring CSV file logging

Examples: `configNtrip`, `printGGAData`, `enableSdCSV`.

## Library Installation

There are two ways to install and use this library:<br>
1. Open the Arduino IDE, go to `Tools -> Manage Libraries`, search for `DFRobot_RTK_LG_EG`, and install it.<br>
2. Download this library manually and copy it into the `Arduino\\libraries` directory. Then open the `examples` folder in the Arduino IDE and run the demo sketches there.<br>

## APIs

```C++
/**
 * @fn begin
 * @brief Initialize the module
 * @param none
 * @return 0: success, 1: fail
 */
uint8_t begin(void);

/**
 * @fn configNtrip
 * @brief Configure NTRIP parameters
 * @param host: NTRIP caster host
 * @param port: NTRIP caster port
 * @param user: user name
 * @param password: password
 * @param mountpoint: mount point
 * @return 0: success, 1: fail
 */
uint8_t configNtrip(String host, String port, String user, String password, String mountpoint);

/**
 * @fn updateGGA
 * @brief Update internal GGA data from the module
 * @param none
 * @return true: success, false: fail
 */
bool updateGGA(void);

/**
 * @fn getTime
 * @brief Get time string from GGA
 * @param none
 * @return time string
 * @note Format: hhmmss.ss
 */
String getTime(void);

/**
 * @fn getLatitude
 * @brief Get latitude
 * @param none
 * @return latitude
 * @note Format: DDMM.MMMM (degrees & minutes)
 */
String getLatitude(void);

/**
 * @fn getLatitudeDirection
 * @brief Get latitude direction
 * @param none
 * @return latitude direction
 * @note Typically \"N\" or \"S\"
 */
String getLatitudeDirection(void);

/**
 * @fn getLongitude
 * @brief Get longitude
 * @param none
 * @return longitude
 * @note Format: DDDMM.MMMM (degrees & minutes)
 */
String getLongitude(void);

/**
 * @fn getLongitudeDirection
 * @brief Get longitude direction
 * @param none
 * @return longitude direction
 * @note Typically \"E\" or \"W\"
 */
String getLongitudeDirection(void);

/**
 * @fn getSatellites
 * @brief Get number of satellites used
 * @param none
 * @return satellite count
 */
String getSatellites(void);

/**
 * @fn getAltitude
 * @brief Get altitude
 * @param none
 * @return altitude in meters
 * @note Altitude in meters
 */
String getAltitude(void);

/**
 * @fn getGeoidSeparation
 * @brief Get geoid separation
 * @param none
 * @return separation in meters
 * @note Geoid separation in meters
 */
String getGeoidSeparation(void);

/**
 * @fn getFixQuality
 * @brief Get fix quality
 * @param none
 * @return fix quality value
 * @note Common values: 0 invalid, 1 GPS, 2 DGPS, 4 RTK fixed, 5 RTK float
 */
String getFixQuality(void);

/**
 * @fn getDiffAge
 * @brief Get differential age
 * @param none
 * @return differential age in seconds
 */
String getDiffAge(void);

/**
 * @fn enableSdCSV
 * @brief Enable saving the selected NMEA sentences to CSV files on the SD card. Multiple flags can be ORed, e.g. eCsvCfgRmc | eCsvCfgGga
 * @param csv_cfg: data type flags (one or more eCSVCfg_t values ORed)
 * @return none
 * @note eCsvCfgAll is 0x40 and means all sentences
 */
void enableSdCSV(uint8_t csv_cfg);

/**
 * @fn disableSdCSV
 * @brief Stop saving the selected NMEA sentences to CSV files on the SD card. Reads the device mask, then sets the given flag bits to 0. Multiple flags can be ORed, e.g. eCsvCfgRmc | eCsvCfgGga
 * @param csv_cfg: data type flags (one or more eCSVCfg_t values ORed)
 * @return 0: success, 1: fail
 */
uint8_t disableSdCSV(uint8_t csv_cfg);

/**
 * @fn deleteCSV
 * @brief Delete stored CSV data for the selected sentence types. Multiple flags can be ORed
 * @param csv_cfg: data type flags (one or more eCSVCfg_t values ORed)
 * @return none
 */
void deleteCSV(uint8_t csv_cfg);
```

## Compatibility

MCU                | Work Well    | Work Wrong | Untested | Remarks
------------------ | :----------: | :--------: | :------: | :-----:
Arduino MEGA2560   |      √       |            |          |
FireBeetle-ESP32   |      √       |            |          |
FireBeetle-M0      |      √       |                 |             |
Leonardo           |      √       |                 |             |

## History

- 2026/02/26 - V1.0.0 released

## Author

Written by Jay Tang (jie.tang@dfrobot.com), 2026. (Welcome to our website)
