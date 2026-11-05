/**
 * @file printGGAData.ino
 * @brief Print GGA data. Centimeter-level RTK needs the configNtrip example first, or the module radio link.
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
}

void loop(){
    if(!rtk.updateGGA()){
        Serial.println("GGA update failed.");
        delay(500);
        return;
    }
    Serial.print("time:");
    Serial.println(rtk.getTime()); // hhmmss.ss
    Serial.print("latitude:");
    Serial.println(rtk.getLatitude()); // DDMM.MMMM
    Serial.print("latitudeDir:");
    Serial.println(rtk.getLatitudeDirection());
    Serial.print("longitude:");
    Serial.println(rtk.getLongitude()); // DDDMM.MMMM
    Serial.print("longitudeDir:");
    Serial.println(rtk.getLongitudeDirection());
    Serial.print("satellites:");
    Serial.println(rtk.getSatellites());
    Serial.print("altitude:");
    Serial.println(rtk.getAltitude());
    Serial.print("geoid separation:");
    Serial.println(rtk.getGeoidSeparation());
    Serial.print("fix quality:");
    Serial.println(rtk.getFixQuality()); // 0 invalid, 1 GPS, 2 DGPS, 4 RTK fixed, 5 RTK float
    Serial.print("diff age:");
    Serial.println(rtk.getDiffAge());
    Serial.println("--------------------------------");
    delay(500);
}
