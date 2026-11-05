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
        CSV_CFG_RMC = 0x01, ///< RMC sentence
        CSV_CFG_GGA = 0x02, ///< GGA sentence
        CSV_CFG_GSV = 0x04, ///< GSV sentence
        CSV_CFG_GSA = 0x08, ///< GSA sentence
        CSV_CFG_VTG = 0x10, ///< VTG sentence
        CSV_CFG_GLL = 0x20, ///< GLL sentence
        CSV_CFG_ALL = 0x40, ///< All sentences. Value is 0x40, not the OR of the other flags
    } eCSVCfg_t;

    /**
     * @struct sGGAData_t
     * @brief GGA data structure
     */
    typedef struct {
        char* UTC;               ///< UTC time of day, hhmmss.ss
        char* Latitude;          ///< Latitude, DDMM.MMMM
        char* LatitudeDirection; ///< Latitude direction, N or S
        char* Longitude;         ///< Longitude, DDDMM.MMMM
        char* LongitudeDirection; ///< Longitude direction, E or W
        char* Satellites;        ///< Satellite count text
        char* Altitude;          ///< Altitude in meters
        char* GeoidSeparation;   ///< Geoid separation in meters
        char* FixQuality;        ///< Fix quality: 0 invalid, 1 GPS, 2 DGPS, 4 RTK fixed, 5 RTK float
        char* DiffAge;           ///< Differential age in seconds
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
     * @param mountpoint: mount point
     * @return none
     */
    void configNtrip(String host, String port, String user, String password, String mountpoint);
    
    /**
     * @fn updateGGA
     * @brief Update GGA data
     * @param none
     * @return true: success, false: failure
     */
    bool updateGGA(void);

    /**
     * @fn getTime
     * @brief Get UTC time of day from GGA
     * @param none
     * @return time string
     * @note Format: hhmmss.ss
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
     * @brief Get the GGA satellite-count field
     * @param none
     * @return satellite count string
     */
    String getSatellites(void);

    /**
     * @fn getAltitude
     * @brief Get altitude
     * @param none
     * @return altitude
     * @note Altitude in meters
     */
    String getAltitude(void);

    /**
     * @fn getGeoidSeparation
     * @brief Get geoid separation
     * @param none
     * @return geoid separation
     * @note Geoid separation in meters
     */
    String getGeoidSeparation(void);

    /**
     * @fn getFixQuality
     * @brief Get fix quality
     * @param none
     * @return fix quality
     * @note Common values: 0 invalid, 1 GPS, 2 DGPS, 4 RTK fixed, 5 RTK float
     */
    String getFixQuality(void);

    /**
     * @fn getDiffAge
     * @brief Get differential age
     * @param none
     * @return differential age
     * @note Differential age in seconds
     */
    String getDiffAge(void);


    /**
     * @fn configCSV
     * @brief Choose which NMEA sentences are written to CSV. OR multiple flags, for example CSV_CFG_RMC | CSV_CFG_GGA
     * @param csv_cfg: one or more eCSVCfg_t values ORed
     * @return none
     */
    void configCSV(uint8_t csv_cfg);

    /**
     * @fn deleteCSV
     * @brief Delete stored CSV data for the selected NMEA sentences. OR multiple flags
     * @param csv_cfg: one or more eCSVCfg_t values ORed
     * @return none
     */
    void deleteCSV(uint8_t csv_cfg);
    

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
