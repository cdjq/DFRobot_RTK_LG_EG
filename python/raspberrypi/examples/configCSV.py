# -*- coding: utf-8 -*-
'''
@file configCSV.py
@brief 配置CSV存储 (Configure CSV logging)
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
SERIAL_PORT = '/dev/ttyAMA0'
BAUD = 115200


def main():
    rtk = DFRobot_RTK_LG_EG(port=SERIAL_PORT, baud=BAUD)
    while rtk.begin() != 0:
        print('RTK init error!')
        time.sleep(1)
    print('RTK init success.')
    # CSV_CFG_RMC 0x01 RMC, CSV_CFG_GGA 0x02 GGA, CSV_CFG_GSV 0x04 GSV
    # CSV_CFG_GSA 0x08 GSA, CSV_CFG_VTG 0x10 VTG, CSV_CFG_GLL 0x20 GLL
    # CSV_CFG_ALL 0x40 all sentences, not the OR of the other flags
    rtk.config_csv(CSV_CFG_RMC | CSV_CFG_GGA)
    # rtk.delete_csv(CSV_CFG_RMC | CSV_CFG_GGA)
    while True:
        time.sleep(1)


if __name__ == '__main__':
    main()
