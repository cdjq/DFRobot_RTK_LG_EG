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
    char buffer[data.length() + 1];
    data.toCharArray(buffer, data.length() + 1);
    DBG(buffer);
    sendCommand(CMD_CONFIG_NTRIP, (uint8_t*)buffer, data.length() + 1);
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
    char *p = ggaPayloadBuf;
    for (int i = 0; i < 16 && p <= ggaPayloadBuf + len; i++) {
        fields[i] = p;
        n++;
        while (*p && p < ggaPayloadBuf + len) p++;
        p++;
    }
    if (n >= 14) {
        ggaData.UTC                = fields[1];
        ggaData.Latitude           = fields[2];
        ggaData.LatitudeDirection  = fields[3];
        ggaData.Longitude          = fields[4];
        ggaData.LongitudeDirection = fields[5];
        ggaData.FixQuality         = fields[6];
        ggaData.Satellites         = fields[7];
        ggaData.Altitude           = fields[9];
        ggaData.GeoidSeparation    = fields[11];
        ggaData.DiffAge            = fields[13];
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
    return ggaData.UTC ? String(ggaData.UTC) : String("");
}

String DFRobot_RTK_LG_EG::getLatitude(void){
    return ggaData.Latitude ? String(ggaData.Latitude) : String("");
}

String DFRobot_RTK_LG_EG::getLatitudeDirection(void){
    return ggaData.LatitudeDirection ? String(ggaData.LatitudeDirection) : String("");
}

String DFRobot_RTK_LG_EG::getLongitude(void){
    return ggaData.Longitude ? String(ggaData.Longitude) : String("");
}

String DFRobot_RTK_LG_EG::getLongitudeDirection(void){
    return ggaData.LongitudeDirection ? String(ggaData.LongitudeDirection) : String("");
}

String DFRobot_RTK_LG_EG::getSatellites(void){
    return ggaData.Satellites ? String(ggaData.Satellites) : String("");
}

String DFRobot_RTK_LG_EG::getAltitude(void){
    return ggaData.Altitude ? String(ggaData.Altitude) : String("");
}

String DFRobot_RTK_LG_EG::getGeoidSeparation(void){
    return ggaData.GeoidSeparation ? String(ggaData.GeoidSeparation) : String("");
}

String DFRobot_RTK_LG_EG::getFixQuality(void){
    return ggaData.FixQuality ? String(ggaData.FixQuality) : String("");
}

String DFRobot_RTK_LG_EG::getDiffAge(void){
    return ggaData.DiffAge ? String(ggaData.DiffAge) : String("");
}


void DFRobot_RTK_LG_EG::enableSdCSV(uint8_t csv_cfg){
    _csvCfg = csv_cfg;
    sendCommand(CMD_CONFIG_CSV, &_csvCfg, 1);
}

uint8_t DFRobot_RTK_LG_EG::disableSdCSV(uint8_t csv_cfg){
    uint8_t data[2];
    sendCommand(CMD_GET_CSV_MASK, NULL, 0);
    int len = receiveResponse(CMD_GET_CSV_MASK, data);
    if (len <= 0)
        return 1;
    _csvCfg = data[0];
    _csvCfg &= (uint8_t)~csv_cfg;
    sendCommand(CMD_CONFIG_CSV, &_csvCfg, 1);
    return 0;
}

void DFRobot_RTK_LG_EG::deleteCSV(uint8_t csv_cfg){
    sendCommand(CMD_CONFIG_DEL_CSV, &csv_cfg, 1);
}


void DFRobot_RTK_LG_EG::sendCommand(uint8_t cmd, uint8_t *data, uint8_t data_len){
    // CRC 计算范围: cmd + payload
    uint8_t crc_buf[256];
    uint8_t crc_len = data_len + 1;
    crc_buf[0] = cmd;
    if (data && data_len > 0) {
        for (uint8_t i = 0; i < data_len; i++)
            crc_buf[i + 1] = data[i];
    }
    uint16_t crc = crc16Ibm(crc_buf, crc_len);
    uint8_t crc_lo = (uint8_t)(crc & 0xFF);
    uint8_t crc_hi = (uint8_t)(crc >> 8);

    // 帧格式: 55 AA len cmd payload crc_lo crc_hi 0D，逐字节写入（无大缓冲区越界风险）
    _serial->write(FRAME_HEAD1);
    _serial->write(FRAME_HEAD2);
    _serial->write(data_len);
    _serial->write(cmd);
    if (data && data_len > 0)
        _serial->write(data, data_len);
    _serial->write(crc_lo);
    _serial->write(crc_hi);
    _serial->write(FRAME_TAIL);
}

int DFRobot_RTK_LG_EG::receiveResponse(uint8_t cmd, uint8_t *data){
    if (!data)
        return -1;
    const unsigned long timeout_ms = VOREO_REQUEST_TIMEOUT;
    unsigned long start = millis();

    // 同步到帧头 55 AA
    while (1) {
        if (_serial->available() < 2) {
            if (millis() - start >= timeout_ms) return -1;
            continue;
        }
        uint8_t b0 = _serial->read();
        if (b0 != FRAME_HEAD1) {
            //DBG(String("skip byte: 0x") + String(b0, HEX));
            continue;
        }
        if (_serial->available() < 1) {
            if (millis() - start >= timeout_ms) return -1;
            continue;
        }
        uint8_t b1 = _serial->read();
        if (b1 == FRAME_HEAD2) break;
    }

    // 读 len、cmd
    while (_serial->available() < 2) {
        if (millis() - start >= timeout_ms) return -1;
    }
    uint8_t len = _serial->read();
    uint8_t cmd_recv = _serial->read();
    DBG(String("recv len=") + String(len) +
        ", cmd_recv=0x" + String(cmd_recv, HEX) +
        ", expect=0x" + String(cmd, HEX));
    if (cmd_recv != cmd) {
        DBG("cmd mismatch, drop frame");
        return -1;
    }

    // 读 payload。len 为单字节，范围 0..255
    uint8_t payload[256];
    for (uint8_t i = 0; i < len; i++) {
            while (!_serial->available()) {
            if (millis() - start >= timeout_ms) return -1;
        }
        payload[i] = _serial->read();
    }

    // 读 crc_lo, crc_hi, tail
    while (_serial->available() < 3) {
        if (millis() - start >= timeout_ms) return -1;
    }
    uint8_t crc_lo = _serial->read();
    uint8_t crc_hi = _serial->read();
    uint8_t tail = _serial->read();
    if (tail != FRAME_TAIL) {
        DBG(String("tail error: 0x") + String(tail, HEX));
        return -1;
    }

    uint16_t crc_recv = (uint16_t)crc_lo | ((uint16_t)crc_hi << 8);
    uint8_t crc_buf[256];
    crc_buf[0] = cmd_recv;
    for (uint8_t i = 0; i < len; i++)
        crc_buf[i + 1] = payload[i];
    uint16_t crc_calc = crc16Ibm(crc_buf, len + 1);
    DBG(String("crc_recv=0x") + String(crc_recv, HEX) +
        ", crc_calc=0x" + String(crc_calc, HEX));
    if (crc_calc != crc_recv) {
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
