/**
 * @file configNtrip.ino
 * @brief Configure NTRIP
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
    rtk.configNtrip("103.45.160.41", "8002", "maplays33280", "66794", "RTCM32GRCpro");
}

void loop(){
    delay(1000);
}
