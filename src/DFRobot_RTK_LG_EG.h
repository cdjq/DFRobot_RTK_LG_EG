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
    #define CMD_CONFIG_NTRIP        0xA3  ///< NTRIP control
    #define CMD_CONFIG_CSV          0xA4  ///< CSV control
    #define CMD_CONFIG_DEL_CSV      0xA5  ///< Delete CSV file
    #define CMD_CONFIG_UART_OUT     0xA6  ///< UART full data output control
    #define CMD_GET_CSV_MASK        0xA7  ///< Get the CSV storage mask from the device
    #define CMD_GET_GGA             0xC1  ///< Get GGA data
    #define BAUD_RATE_115200        115200 ///< Baud rate
    #define VOREO_REQUEST_TIMEOUT   10000 ///< Request timeout (ms)

    /**
     * @enum eCSVCfg_t
     * @brief CSV data type
     */
    typedef enum {
        eCsvCfgRmc = 0x01, ///< RMC sentence
        eCsvCfgGga = 0x02, ///< GGA sentence
        eCsvCfgGsv = 0x04, ///< GSV sentence
        eCsvCfgGsa = 0x08, ///< GSA sentence
        eCsvCfgVtg = 0x10, ///< VTG sentence
        eCsvCfgGll = 0x20, ///< GLL sentence
        eCsvCfgAll = 0x40, ///< All sentences. Value is 0x40, not the OR of the other flags
    } eCSVCfg_t;

    /**
     * @struct sGGAData_t
     * @brief GGA data structure
     */
    typedef struct {
        char* utc;               ///< UTC time of day, hhmmss.ss
        char* latitude;          ///< Latitude, DDMM.MMMM
        char* latitudeDirection; ///< Latitude direction, N or S
        char* longitude;         ///< Longitude, DDDMM.MMMM
        char* longitudeDirection; ///< Longitude direction, E or W
        char* satellites;        ///< Satellite count text
        char* altitude;          ///< Altitude in meters
        char* geoidSeparation;   ///< Geoid separation in meters
        char* fixQuality;        ///< Fix quality: 0 invalid, 1 GPS, 2 DGPS, 4 RTK fixed, 5 RTK float
        char* diffAge;           ///< Differential age in seconds
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
     * @return 0: success, 1: failure
     */
    uint8_t configNtrip(String host, String port, String user, String password, String mountpoint);
    
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
     * @fn enableSdCSV
     * @brief Enable saving the selected NMEA sentences to CSV files on the SD card. OR multiple flags, for example eCsvCfgRmc | eCsvCfgGga
     * @param csvCfg: one or more eCSVCfg_t values ORed
     * @return none
     */
    void enableSdCSV(uint8_t csvCfg);

    /**
     * @fn disableSdCSV
     * @brief Stop saving the selected NMEA sentences to CSV files on the SD card. Reads the device mask, then sets the given flag bits to 0. OR multiple flags, for example eCsvCfgRmc | eCsvCfgGga
     * @param csvCfg: one or more eCSVCfg_t values ORed
     * @return 0: success, 1: failure
     */
    uint8_t disableSdCSV(uint8_t csvCfg);

    /**
     * @fn deleteCSV
     * @brief Delete stored CSV data for the selected NMEA sentences. OR multiple flags
     * @param csvCfg: one or more eCSVCfg_t values ORed
     * @return none
     */
    void deleteCSV(uint8_t csvCfg);
    

private:
    HardwareSerial *_serial;
    uint8_t _csvCfg;
    void sendCommand(uint8_t cmd, uint8_t *data, uint8_t dataLen);
    /** Receive one response frame, using the same protocol as sendCommand.
     *  @param cmd   Expected command byte (must match frame cmd, otherwise -1 is returned)
     *  @param data  Payload buffer (must be at least 256 bytes)
     *  @return Payload length on success, -1 on failure
     */
    int receiveResponse(uint8_t cmd, uint8_t *data);

};

#endif
