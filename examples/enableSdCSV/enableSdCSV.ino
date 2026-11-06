/**
 * @file enableSdCSV.ino
 * @brief Enable saving NMEA sentences to CSV files on the SD card
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
    // eCsvCfgRmc 0x01 RMC, eCsvCfgGga 0x02 GGA, eCsvCfgGsv 0x04 GSV
    // eCsvCfgGsa 0x08 GSA, eCsvCfgVtg 0x10 VTG, eCsvCfgGll 0x20 GLL
    // eCsvCfgAll 0x40 all sentences, not the OR of the other flags
    rtk.enableSdCSV(rtk.eCsvCfgRmc|rtk.eCsvCfgGga);
    // rtk.disableSdCSV(rtk.eCsvCfgRmc|rtk.eCsvCfgGga);
    // rtk.deleteCSV(rtk.eCsvCfgRmc|rtk.eCsvCfgGga);
}

void loop(){
    delay(1000);
}
