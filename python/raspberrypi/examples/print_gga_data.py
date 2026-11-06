# -*- coding: utf-8 -*-
'''
@file print_gga_data.py
@brief Print GGA data. Centimeter-level RTK needs config_ntrip.py first, or the module radio link.
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

rtk = None


def setup():
    global rtk
    rtk = DFRobot_RTK_LG_EG(port=SERIAL_PORT, baud=BAUD)
    while rtk.begin() != 0:
        print('RTK init error!')
        time.sleep(1)
    print('RTK init success.')


def loop():
    if not rtk.update_gga():
        print('GGA update failed.')
        time.sleep(0.5)
        return
    print('time:', rtk.get_time())  # hhmmss.ss
    print('latitude:', rtk.get_latitude())  # DDMM.MMMM
    print('latitudeDir:', rtk.get_latitude_direction())
    print('longitude:', rtk.get_longitude())  # DDDMM.MMMM
    print('longitudeDir:', rtk.get_longitude_direction())
    print('satellites:', rtk.get_satellites())
    print('altitude:', rtk.get_altitude())
    print('geoid separation:', rtk.get_geoid_separation())
    print('fix quality:', rtk.get_fix_quality())  # 0 invalid, 1 GPS, 2 DGPS, 4 RTK fixed, 5 RTK float
    print('diff age:', rtk.get_diff_age())
    print('--------------------------------')
    time.sleep(0.5)


if __name__ == '__main__':
    setup()
    while True:
        loop()
