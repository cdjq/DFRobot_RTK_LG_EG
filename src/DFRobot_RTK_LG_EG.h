/**
 * @file DFRobot_RTK_LG_EG.h
 * @brief DFRobot RTK LG/EG library for Arduino, UART protocol (55 AA len cmd payload crc 0D)
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author Jay Tang(jie.tang@dfrobot.com)
 * @version V1.0
 * @date 2026-02-26
 * @url https://github.com/DFRobot/DFRobot_RTK_LG_EG
 */

#ifndef __DFROBOT_RTK_LG_EG_H__
#define __DFROBOT_RTK_LG_EG_H__
#include "Arduino.h"

//#define ENABLE_DBG ///< Enable this macro to see the detailed running process of the program
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

class DFRobot_RTK_LG_EG {
public:
    #define CMD_HANDSHAKE           0xA0  ///< Handshake
    #define CMD_CONFIG_GNSS         0xA1  ///< GNSS control
    #define CMD_CONFIG_4G           0xA2  ///< 4G control
    #define CMD_CONFIG_NTRIP        0xA3  ///< NTRIP control
    #define CMD_CONFIG_CSV          0xA4  ///< CSV control
    #define CMD_CONFIG_DEL_CSV      0xA5  ///< Delete CSV file
    #define CMD_CONFIG_UART_OUT     0xA6  ///< UART full data output control
    #define CMD_GET_GGA             0xC1  ///< Get GGA data
    #define BAUD_RATE_115200        115200 ///< Baud rate
    #define VOREO_REQUEST_TIMEOUT   10000 ///< Request timeout (ms)

    /**
     * @enum eCSVCfg_t
     * @brief CSV data type
     */
    typedef enum {
        SLAVE_CFG_RMC = 0x01, ///< RMC data
        SLAVE_CFG_GGA = 0x02, ///< GGA data
        SLAVE_CFG_GSV = 0x04, ///< GSV data
        SLAVE_CFG_GSA = 0x08, ///< GSA data
        SLAVE_CFG_VTG = 0x10, ///< VTG data
        SLAVE_CFG_GLL = 0x20, ///< GLL data
        SLAVE_CFG_ALL = 0x40, ///< All data
    } eCSVCfg_t;

    /**
     * @struct sGGAData_t
     * @brief GGA data structure
     */
    typedef struct {
        char* UTC;               ///< UTC time
        char* Latitude;          ///< Latitude
        char* LatitudeDirection; ///< Latitude direction
        char* Longitude;         ///< Longitude
        char* LongitudeDirection; ///< Longitude direction
        char* Satellites;        ///< Number of satellites
        char* Altitude;          ///< Altitude
        char* GeoidSeparation;   ///< Geoid separation
        char* FixQuality;        ///< Fix quality
        char* DiffAge;           ///< Differential age
    } sGGAData_t;

    /**
     * @fn DFRobot_RTK_LG_EG
     * @brief Constructor
     * @param serial: serial port object
     * @return none
     */
    DFRobot_RTK_LG_EG(HardwareSerial *serial = &Serial1);

    /**
     * @fn begin
     * @brief Initialize the module
     * @param none
     * @return 0: success, 1: failure
     */
    uint8_t begin(void);

    /**
     * @fn configNtrip
     * @brief Configure NTRIP
     * @param host: host address
     * @param port: port number
     * @param user: user name
     * @param password: password
     * * @param mountpoint: mount point
     * @return none
     */
    void configNtrip(String host, String port, String user, String password, String mountpoint);
    
    /**
     * @fn update
     * @brief Update GGA data
     * @param none
     * @return true: success, false: failure
     */
    bool updateGGA(void);

    /**
     * @fn getTime
     * @brief Get time
     * @param none
     * @return time string
     * @note Format: YYYY-MM-DD HH:MM:SS
     */
    String getTime(void);
    /**
     * @fn getLatitude
     * @brief Get latitude
     * @param none
     * @return latitude
     * @note Format: DDMM.MMMM (degrees & minutes)
     */
    String getLatitude(void);
    /**
     * @fn getLatitudeDirection
     * @brief Get latitude direction
     * @param none
     * @return latitude direction
     * @note Typically 'N' or 'S'
     */
    String getLatitudeDirection(void);
    /**
     * @fn getLongitude
     * @brief Get longitude
     * @param none
     * @return longitude
     * @note Format: DDDMM.MMMM (degrees & minutes)
     */
    String getLongitude(void);
    /**
     * @fn getLongitudeDirection
     * @brief Get longitude direction
     * @param none
     * @return longitude direction
     * @note Typically 'E' or 'W'
     */
    String getLongitudeDirection(void);
    /**
     * @fn getSatellites
     * @brief Get number of satellites
     * @param none
     * @return satellite count
     * @note 0: none, 1: 1 satellite, 2: 2 satellites, 3: 3 satellites, 4: 4 satellites, 5: 5 satellites, 6: 6 satellites, 7: 7 satellites, 8: 8 satellites, 9: 9 satellites, 10: 10 satellites, 11: 11 satellites, 12: 12 satellites, 13: 13 satellites, 14: 14 satellites, 15: 15 satellites, 16: 16 satellites, 17: 17 satellites, 18: 18 satellites, 19: 19 satellites, 20: 20 satellites
     */
    String getSatellites(void);
    /**
     * @fn getAltitude
     * @brief Get altitude
     * @param none
     * @return altitude
     * @note Format: altitude (m)
     */
    String getAltitude(void);
    /**
     * @fn getGeoidSeparation
     * @brief Get geoid separation
     * @param none
     * @return geoid separation
     * @note Format: separation (m)
     */
    String getGeoidSeparation(void);
    /**
     * @fn getFixQuality
     * @brief Get fix quality
     * @param none
     * @return fix quality
     */
    String getFixQuality(void);
    /**
     * @fn getDiffAge
     * @brief Get differential age
     * @param none
     * @return differential age
     * @note Unit: seconds
     */
    String getDiffAge(void);


    /**
     * @fn CSVSlaveCfg
     * @brief Configure CSV data types; multiple flags can be ORed, e.g. SLAVE_CFG_RMC | SLAVE_CFG_GGA
     * @param slave_cfg: data type flags (one or more eCSVCfg_t values ORed)
     * @return none
     */
    void CSVSlaveCfg(uint8_t slave_cfg);

    /**
     * @fn CSVDelCfg
     * @brief Delete CSV data types; multiple flags can be ORed
     * @param del_cfg: data type flags (one or more eCSVCfg_t values ORed)
     * @return none
     */
    void CSVDelCfg(uint8_t del_cfg);
    

private:
    HardwareSerial *_serial;
    void sendCommand(uint8_t cmd, uint8_t *data, uint8_t data_len);
    /** Receive one response frame, using the same protocol as sendCommand.
     *  @param cmd   Expected command byte (must match frame cmd, otherwise -1 is returned)
     *  @param data  Payload buffer (must be at least 256 bytes)
     *  @return Payload length on success, -1 on failure
     */
    int receiveResponse(uint8_t cmd, uint8_t *data);

};

#endif
