/**
 * @file DFRobot_RTK_LG_EG.cpp
 * @brief DFRobot RTK LG/EG library for Arduino, UART protocol (55 AA len cmd payload crc 0D)
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author Jay Tang(jie.tang@dfrobot.com)
 * @version V1.0
 * @date 2026-02-26
 * @url https://github.com/DFRobot/DFRobot_RTK_LG_EG
 */
#include "DFRobot_RTK_LG_EG.h"
#include <string.h>

#define FRAME_HEAD1  0x55
#define FRAME_HEAD2  0xAA
#define FRAME_TAIL   0x0D

static DFRobot_RTK_LG_EG::sGGAData_t ggaData;
/** 解析出的 GGA 字段指针指向此缓冲区（逗号被替换为 \\0） */
static char ggaPayloadBuf[256];

/** CRC-16/IBM: init 0xFFFF, poly 0xA001, LSB-first */
static uint16_t crc16Ibm(uint8_t *data, uint8_t len) {
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
        crc &= 0xFFFF;
    }
    return crc;
}

DFRobot_RTK_LG_EG::DFRobot_RTK_LG_EG(HardwareSerial *serial) : _serial(serial), _csvCfg(0) {}

uint8_t DFRobot_RTK_LG_EG::begin(void){
    uint8_t data[2];
    
    #ifdef ESP32
    _serial->begin(BAUD_RATE_115200, SERIAL_8N1, D3, D2);
    delay(100);
    #else
        _serial->begin(BAUD_RATE_115200);    // M0 cannot create a begin in a construct
    #endif
    sendCommand(CMD_HANDSHAKE, NULL, 0);
    int len = receiveResponse(CMD_HANDSHAKE, data);
    DBG(String("recv len=") + String(data[0]));
    if (len <= 0)
        return 1;
    if (data[0] != 0x01)
        return 1;
    return 0;
}

uint8_t DFRobot_RTK_LG_EG::configNtrip(String host, String port, String user, String password, String mountpoint){
    uint8_t resp[2];
    String data = host + "," + port + ","  + user + "," + password + "," + mountpoint;
    DBG(data.c_str());
    sendCommand(CMD_CONFIG_NTRIP, (uint8_t*)data.c_str(), data.length() + 1);
    int len = receiveResponse(CMD_CONFIG_NTRIP, resp);
    if (len <= 0)
        return 1;
    if (resp[0] != 0x01)
        return 1;
    return 0;
}


/** NMEA GGA 字段顺序: 0=$GxGGA, 1=UTC, 2=Lat, 3=N/S, 4=Lon, 5=E/W, 6=Quality, 7=Satellites, 8=HDOP, 9=Alt, 10=M, 11=GeoidSep, 12=M, 13=DiffAge, 14=DiffStation, 15=*CS。解析结果写入 ggaData。 */
static void parseGGA(uint8_t *data, int len) {
    if (len <= 0 || len >= (int)sizeof(ggaPayloadBuf))
        return;
    memcpy(ggaPayloadBuf, data, len);
    ggaPayloadBuf[len] = '\0';
    for (int i = 0; i < len; i++) {
        if (ggaPayloadBuf[i] == ',')
            ggaPayloadBuf[i] = '\0';
    }
    char *fields[16];
    int n = 0;
    char *pField = ggaPayloadBuf;
    for (int i = 0; i < 16 && pField <= ggaPayloadBuf + len; i++) {
        fields[i] = pField;
        n++;
        while (*pField && pField < ggaPayloadBuf + len) pField++;
        pField++;
    }
    if (n >= 14) {
        ggaData.utc                = fields[1];
        ggaData.latitude           = fields[2];
        ggaData.latitudeDirection  = fields[3];
        ggaData.longitude          = fields[4];
        ggaData.longitudeDirection = fields[5];
        ggaData.fixQuality         = fields[6];
        ggaData.satellites         = fields[7];
        ggaData.altitude           = fields[9];
        ggaData.geoidSeparation    = fields[11];
        ggaData.diffAge            = fields[13];
    }
}

bool DFRobot_RTK_LG_EG::updateGGA(void){
    uint8_t data[256];
    sendCommand(CMD_GET_GGA, NULL, 0);
    int len = receiveResponse(CMD_GET_GGA, data);
    if (len <= 0)
        return false;
    parseGGA(data, len);
    return true;
}


String DFRobot_RTK_LG_EG::getTime(void){
    return ggaData.utc ? String(ggaData.utc) : String("");
}

String DFRobot_RTK_LG_EG::getLatitude(void){
    return ggaData.latitude ? String(ggaData.latitude) : String("");
}

String DFRobot_RTK_LG_EG::getLatitudeDirection(void){
    return ggaData.latitudeDirection ? String(ggaData.latitudeDirection) : String("");
}

String DFRobot_RTK_LG_EG::getLongitude(void){
    return ggaData.longitude ? String(ggaData.longitude) : String("");
}

String DFRobot_RTK_LG_EG::getLongitudeDirection(void){
    return ggaData.longitudeDirection ? String(ggaData.longitudeDirection) : String("");
}

String DFRobot_RTK_LG_EG::getSatellites(void){
    return ggaData.satellites ? String(ggaData.satellites) : String("");
}

String DFRobot_RTK_LG_EG::getAltitude(void){
    return ggaData.altitude ? String(ggaData.altitude) : String("");
}

String DFRobot_RTK_LG_EG::getGeoidSeparation(void){
    return ggaData.geoidSeparation ? String(ggaData.geoidSeparation) : String("");
}

String DFRobot_RTK_LG_EG::getFixQuality(void){
    return ggaData.fixQuality ? String(ggaData.fixQuality) : String("");
}

String DFRobot_RTK_LG_EG::getDiffAge(void){
    return ggaData.diffAge ? String(ggaData.diffAge) : String("");
}


void DFRobot_RTK_LG_EG::enableSdCSV(uint8_t csvCfg){
    _csvCfg = csvCfg;
    sendCommand(CMD_CONFIG_CSV, &_csvCfg, 1);
}

uint8_t DFRobot_RTK_LG_EG::disableSdCSV(uint8_t csvCfg){
    uint8_t data[2];
    sendCommand(CMD_GET_CSV_MASK, NULL, 0);
    int len = receiveResponse(CMD_GET_CSV_MASK, data);
    if (len <= 0)
        return 1;
    _csvCfg = data[0];
    _csvCfg &= (uint8_t)~csvCfg;
    sendCommand(CMD_CONFIG_CSV, &_csvCfg, 1);
    return 0;
}

void DFRobot_RTK_LG_EG::deleteCSV(uint8_t csvCfg){
    sendCommand(CMD_CONFIG_DEL_CSV, &csvCfg, 1);
}


void DFRobot_RTK_LG_EG::sendCommand(uint8_t cmd, uint8_t *data, uint8_t dataLen){
    // CRC 计算范围: cmd + payload
    uint8_t crcBuf[256];
    uint8_t crcLen = dataLen + 1;
    crcBuf[0] = cmd;
    if (data && dataLen > 0) {
        for (uint8_t i = 0; i < dataLen; i++)
            crcBuf[i + 1] = data[i];
    }
    uint16_t crc = crc16Ibm(crcBuf, crcLen);
    uint8_t crcLo = (uint8_t)(crc & 0xFF);
    uint8_t crcHi = (uint8_t)(crc >> 8);

    // 帧格式: 55 AA len cmd payload crcLo crcHi 0D，逐字节写入（无大缓冲区越界风险）
    _serial->write(FRAME_HEAD1);
    _serial->write(FRAME_HEAD2);
    _serial->write(dataLen);
    _serial->write(cmd);
    if (data && dataLen > 0)
        _serial->write(data, dataLen);
    _serial->write(crcLo);
    _serial->write(crcHi);
    _serial->write(FRAME_TAIL);
}

int DFRobot_RTK_LG_EG::receiveResponse(uint8_t cmd, uint8_t *data){
    if (!data)
        return -1;
    const unsigned long timeoutMs = VOREO_REQUEST_TIMEOUT;
    unsigned long start = millis();

    // 同步到帧头 55 AA
    while (1) {
        if (_serial->available() < 2) {
            if (millis() - start >= timeoutMs) return -1;
            continue;
        }
        uint8_t b0 = _serial->read();
        if (b0 != FRAME_HEAD1) {
            //DBG(String("skip byte: 0x") + String(b0, HEX));
            continue;
        }
        if (_serial->available() < 1) {
            if (millis() - start >= timeoutMs) return -1;
            continue;
        }
        uint8_t b1 = _serial->read();
        if (b1 == FRAME_HEAD2) break;
    }

    // 读 len、cmd
    while (_serial->available() < 2) {
        if (millis() - start >= timeoutMs) return -1;
    }
    uint8_t len = _serial->read();
    uint8_t cmdRecv = _serial->read();
    DBG(String("recv len=") + String(len) +
        ", cmdRecv=0x" + String(cmdRecv, HEX) +
        ", expect=0x" + String(cmd, HEX));
    if (cmdRecv != cmd) {
        DBG("cmd mismatch, drop frame");
        return -1;
    }

    // 读 payload。len 为单字节，范围 0..255
    uint8_t payload[256];
    for (uint8_t i = 0; i < len; i++) {
            while (!_serial->available()) {
            if (millis() - start >= timeoutMs) return -1;
        }
        payload[i] = _serial->read();
    }

    // 读 crcLo, crcHi, tail
    while (_serial->available() < 3) {
        if (millis() - start >= timeoutMs) return -1;
    }
    uint8_t crcLo = _serial->read();
    uint8_t crcHi = _serial->read();
    uint8_t tail = _serial->read();
    if (tail != FRAME_TAIL) {
        DBG(String("tail mismatch: 0x") + String(tail, HEX));
        return -1;
    }

    uint16_t crcRecv = (uint16_t)crcLo | ((uint16_t)crcHi << 8);
    uint8_t crcBuf[256];
    crcBuf[0] = cmdRecv;
    for (uint8_t i = 0; i < len; i++)
        crcBuf[i + 1] = payload[i];
    uint16_t crcCalc = crc16Ibm(crcBuf, len + 1);
    DBG(String("crcRecv=0x") + String(crcRecv, HEX) +
        ", crcCalc=0x" + String(crcCalc, HEX));
    if (crcCalc != crcRecv) {
        DBG("CRC mismatch, drop frame");
        return -1;
    }

    // 仅将 payload 拷贝到 data（不包含帧头、cmd、CRC、尾）
    memcpy(data, payload, len);

    // 打印 payload 为可读字符串/十六进制
    String hex;
    for (uint8_t i = 0; i < len; i++) {
        if (i) hex += " ";
        if (payload[i] < 16) hex += "0";
        hex += String(payload[i], HEX);
    }
    DBG(String("payload(len=") + String(len) + "): " + hex);
    return (int)len;
}
