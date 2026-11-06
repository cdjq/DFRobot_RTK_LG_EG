# -*- coding: utf-8 -*-
'''
@file config_ntrip.py
@brief Configure NTRIP
@copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@license The MIT License (MIT)
@version V1.0.0
@date 2026-02-26
@url https://github.com/DFRobot/DFRobot_RTK_LG_EG
'''

import sys
import time

sys.path.append("../")
from DFRobot_RTK_LG_EG import DFRobot_RTK_LG_EG

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
  while rtk.config_ntrip(
    host='103.45.160.41',
    port='8002',
    user='maplays33280',
    password='66794',
    mountpoint='RTCM32GRCpro'
  ) != 0:
    print('NTRIP config error!')
    time.sleep(1)
  print('NTRIP config success.')


def loop():
  time.sleep(1)


if __name__ == '__main__':
  setup()
  while True:
    loop()
