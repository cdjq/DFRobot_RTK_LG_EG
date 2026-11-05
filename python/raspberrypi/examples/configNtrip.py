# -*- coding: utf-8 -*-
'''
@file configNtrip.py
@brief 配置NTRIP (Configure NTRIP)
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
SERIAL_PORT = '/dev/ttyAMA0'
BAUD = 115200


def main():
    rtk = DFRobot_RTK_LG_EG(port=SERIAL_PORT, baud=BAUD)
    while rtk.begin() != 0:
        print('RTK init error!')
        time.sleep(1)
    print('RTK init success.')
    rtk.config_ntrip(
        host='103.45.160.41',
        port='8002',
        user='maplays33280',
        password='66794',
        mountpoint='RTCM32GRCpro'
    )
    while True:
        time.sleep(1)


if __name__ == '__main__':
    main()
