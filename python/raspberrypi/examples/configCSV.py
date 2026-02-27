# -*- coding: utf-8 -*-
'''
@file configCSV.py
@brief Config CSV data storage
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
from DFRobot_RTK_LG_EG import DFRobot_RTK_LG_EG, SLAVE_CFG_RMC, SLAVE_CFG_GGA

# Raspberry Pi: /dev/ttyAMA0 or /dev/ttyS0; USB: /dev/ttyUSB0
SERIAL_PORT = '/dev/ttyAMA0'
BAUD = 115200


def main():
    rtk = DFRobot_RTK_LG_EG(port=SERIAL_PORT, baud=BAUD)
    while rtk.begin() != 0:
        print('RTK init error!')
        time.sleep(1)
    print('RTK init success.')
    # 配置CSV存储，可多选：SLAVE_CFG_RMC|SLAVE_CFG_GGA 等
    rtk.csv_slave_cfg(SLAVE_CFG_RMC | SLAVE_CFG_GGA)
    # 删除CSV文件 (uncomment to delete CSV)
    # rtk.csv_del_cfg(SLAVE_CFG_RMC | SLAVE_CFG_GGA)
    while True:
        time.sleep(1)


if __name__ == '__main__':
    main()
