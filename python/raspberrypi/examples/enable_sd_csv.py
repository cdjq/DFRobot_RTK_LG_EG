# -*- coding: utf-8 -*-
'''
@file enable_sd_csv.py
@brief Enable saving NMEA sentences to CSV files on the SD card
@copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@license The MIT License (MIT)
@version V1.0.0
@date 2026-02-26
@url https://github.com/DFRobot/DFRobot_RTK_LG_EG
'''

import sys
import time

sys.path.append("../")
from DFRobot_RTK_LG_EG import DFRobot_RTK_LG_EG, CSV_CFG_RMC, CSV_CFG_GGA

# Raspberry Pi: /dev/ttyAMA0 or /dev/ttyS0; USB: /dev/ttyUSB0
serial_port = '/dev/ttyAMA0'
baud_rate = 115200

rtk = None


def setup():
  global rtk  # pylint: disable=global-statement
  rtk = DFRobot_RTK_LG_EG(port=serial_port, baud=baud_rate)
  while rtk.begin() != 0:
    print('RTK init error!')
    time.sleep(1)
  print('RTK init success.')
  # CSV_CFG_RMC 0x01 RMC, CSV_CFG_GGA 0x02 GGA, CSV_CFG_GSV 0x04 GSV
  # CSV_CFG_GSA 0x08 GSA, CSV_CFG_VTG 0x10 VTG, CSV_CFG_GLL 0x20 GLL
  # CSV_CFG_ALL 0x40 all sentences, not the OR of the other flags
  rtk.enable_sd_csv(CSV_CFG_RMC | CSV_CFG_GGA)
  # rtk.disable_sd_csv(CSV_CFG_RMC | CSV_CFG_GGA)
  # rtk.delete_csv(CSV_CFG_RMC | CSV_CFG_GGA)


def loop():
  time.sleep(1)


if __name__ == '__main__':
  setup()
  while True:
    loop()
