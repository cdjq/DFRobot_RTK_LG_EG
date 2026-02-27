# -*- coding: utf-8 -*-
'''
@file printGGAData.py
@brief 打印GGA数据 (Print GGA data)
@copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@license The MIT License (MIT)
@version V1.0.0
@date 2026-02-26
@url https://github.com/DFRobot/DFRobot_RTK_LG_EG
'''

import sys
import os
import time

# Add parent path so we can import DFRobot_RTK_LG_EG
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
    # 配置NTRIP (Config NTRIP)
    rtk.config_ntrip(
        host='103.45.160.41',
        port='8002',
        user='maplays33280',
        password='66794',
        mountpoint='RTCM32GRCpro'
    )

    while True:
        rtk.update_gga()
        print('time:', rtk.get_time())
        print('latitude:', rtk.get_latitude())
        print('latitudeDir:', rtk.get_latitude_direction())
        print('longitude:', rtk.get_longitude())
        print('longitudeDir:', rtk.get_longitude_direction())
        print('satellites:', rtk.get_satellites())
        print('altitude:', rtk.get_altitude())
        print('geoid separation:', rtk.get_geoid_separation())
        print('fix quality:', rtk.get_fix_quality())
        print('diff age:', rtk.get_diff_age())
        print('--------------------------------')
        time.sleep(0.5)


if __name__ == '__main__':
    main()
