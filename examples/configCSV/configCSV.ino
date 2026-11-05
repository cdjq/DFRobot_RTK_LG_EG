/**
 * @file configCSV.ino
 * @brief Configure CSV data logging
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author Jay Tang(jie.tang@dfrobot.com)
 * @version V1.0.0
 * @date 2026-02-26
 * @url https://github.com/DFRobot/DFRobot_RTK_LG_EG
 */

#include "DFRobot_RTK_LG_EG.h"
/* -----------------------------------------------------------
 * |  Sensor  | Connect line | Leonardo/Mega2560/M0 | ESP32 |
 * |   VCC    |=============>|        VCC           |  VCC  |
 * |   GND    |=============>|        GND           |  GND  |
 * |   RX     |=============>|     Serial1 TX1      |  D2   |
 * |   TX     |=============>|     Serial1 RX1      |  D3   |
 * -----------------------------------------------------------*/
DFRobot_RTK_LG_EG rtk(&Serial1);

void setup(){
    Serial.begin(115200);
    while(rtk.begin() != 0){
        Serial.println("RTK init error!");
        delay(1000);
    }
    Serial.println("RTK init success.");
    // CSV_CFG_RMC 0x01 RMC, CSV_CFG_GGA 0x02 GGA, CSV_CFG_GSV 0x04 GSV
    // CSV_CFG_GSA 0x08 GSA, CSV_CFG_VTG 0x10 VTG, CSV_CFG_GLL 0x20 GLL
    // CSV_CFG_ALL 0x40 all sentences, not the OR of the other flags
    rtk.configCSV(rtk.CSV_CFG_RMC|rtk.CSV_CFG_GGA);
    // rtk.deleteCSV(rtk.CSV_CFG_RMC|rtk.CSV_CFG_GGA);
}

void loop(){
    delay(1000);
}
