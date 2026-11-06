# DFRobot_RTK_LG_EG
- [Chinese Version](./README_CN.md)

This is a high‑precision GNSS‑RTK positioning device designed for enterprise‑level applications, capable of centimeter‑level outdoor positioning accuracy.  
The product integrates a 4G communication module and supports quick access to RTK differential services via the Ntrip protocol. It can also connect to an external wireless data link module to achieve high‑accuracy positioning, adapting flexibly to a variety of deployment environments.

The device provides rich interfaces including UART and USB, making it easy to integrate into systems and extend functionality. It supports fast data validation on a host computer, has power‑down data retention, and can be expanded with an SD card to ensure important data is not lost.

The core module is Quectel LG290P03AAMD, supporting multi‑constellation and multi‑frequency reception with an output rate up to 20 Hz (10 Hz by default). With built‑in interference detection and suppression algorithms, it can still maintain accurate positioning even in complex electromagnetic environments or urban canyon scenarios.

This product is suitable for intelligent robot navigation, precision agriculture, industrial‑park AGV positioning robots, and more. It is an ideal choice for enterprise users to quickly validate positioning capability and integrate RTK into their applications.

![Module image](../../resources/images/TEL0193.png)

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

This directory provides a Python library for the RTK module on Raspberry Pi. The library mainly supports:

- Reading GNSS GGA data  
- Configuring CSV file logging

Examples: `config_ntrip.py`, `print_gga_data.py`, `enable_sd_csv.py`.

## Library Installation
1. Download this repository to your Raspberry Pi. To clone via git:

```
sudo git clone https://github.com/DFRobot/DFRobot_RTK_LG_EG
```

2. Open and run an example. To run a demo `demo_x.py`, type `python demo_x.py` in the terminal. For example, to run the `print_gga_data.py` demo, you can use:

```
python  print_gga_data.py
or
python2 print_gga_data.py
or
python3 print_gga_data.py
```

## APIs

```python

  def begin(self):
        '''!
        @brief Init and handshake
        @return 0: success, 1: fail
        '''

    def config_ntrip(self, host, port, user, password, mountpoint):
        '''!
        @brief Config NTRIP: host, port, user, password, mountpoint
        @param host: server host
        @param port: port string or int
        @param user: user name
        @param password: password
        @param mountpoint: mount point
        @return 0: success, 1: fail
        '''
    def update_gga(self):
        '''!
        @brief Request and update GGA data
        @return True on success, False on fail
        '''

    def get_time(self):
        '''!
        @brief Get UTC time of day from GGA
        @return time string, format hhmmss.ss
        '''

    def get_latitude(self):
        '''!
        @brief Get latitude from GGA
        @return latitude string, format DDMM.MMMM
        '''

    def get_latitude_direction(self):
        '''!
        @brief Get latitude direction
        @return 'N' or 'S' or ''
        '''

    def get_longitude(self):
        '''!
        @brief Get longitude from GGA
        @return longitude string, format DDDMM.MMMM
        '''

    def get_longitude_direction(self):
        '''!
        @brief Get longitude direction
        @return 'E' or 'W' or ''
        '''

    def get_satellites(self):
        '''!
        @brief Get the GGA satellite-count field
        @return satellite count string
        '''

    def get_altitude(self):
        '''!
        @brief Get altitude from GGA
        @return altitude string in meters
        '''

    def get_geoid_separation(self):
        '''!
        @brief Get geoid separation from GGA
        @return geoid separation string in meters
        '''

    def get_fix_quality(self):
        '''!
        @brief Get GGA fix quality
        @return fix quality string. Common values: 0 invalid, 1 GPS, 2 DGPS, 4 RTK fixed, 5 RTK float
        '''

    def get_diff_age(self):
        '''!
        @brief Get differential age from GGA
        @return differential age string in seconds
        '''

    def enable_sd_csv(self, csv_cfg):
        '''!
        @brief Enable saving the selected NMEA sentences to CSV files on the SD card. OR flags, for example CSV_CFG_RMC | CSV_CFG_GGA
        @param csv_cfg: one or more CSV_CFG_* flags ORed
        @note CSV_CFG_ALL is 0x40 and means all sentences
        '''

    def disable_sd_csv(self, csv_cfg):
        '''!
        @brief Stop saving the selected NMEA sentences to CSV files on the SD card. Read the device mask, then set the given flag bits to 0.
        @param csv_cfg: one or more CSV_CFG_* flags ORed
        @return 0: success, 1: fail
        '''

    def delete_csv(self, csv_cfg):
        '''!
        @brief Delete stored CSV data for the selected NMEA sentences
        @param csv_cfg: one or more CSV_CFG_* flags ORed
        '''
```

## Compatibility

* Raspberry Pi versions

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | :-----: |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python versions

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | :-----: |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |

## History

- 2026/02/26 - V1.0.0 released

## Author

Written by Jay Tang(jie.tang@dfrobot.com), 2026. (Welcome to our website)
