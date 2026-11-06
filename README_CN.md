# DFRobot_RTK_LG_EG
- [English Version](./README.md)
这是一款专为企业级应用设计的高精度GNSS-RTK定位设备，能够实现户外厘米级精度的定位。产品内置4G通信模块，支持通过Ntrip协议快速接入RTK差分服务，也可外接无线数传模块实现高精度定位，灵活适应多种部署环境。
设备集成丰富接口，包括UART、USB等，便于系统集成与扩展。支持上位机快速数据验证，具备掉电数据保存功能，并可通过SD卡扩展存储，确保关键数据不丢失。
核心采用移远LG290P03AAMD模组，支持全星系多频段信号接收，最高输出频率可达20Hz（默认10Hz）。内置干扰检测与抑制算法，即使在电磁环境复杂或城市峡谷等挑战性场景中，依旧能保证定位的准确性。
该产品适用于智能机器人导航、精准农业作业、工业园区智能AGV定位机器人等等，是企业用户快速验证定位功能和集成项目应用的理想选择。

![正反面svg效果图](/resources/images/TEL0193.png)

## 产品链接(https://www.dfrobot.com)

    SKU：TEL0193

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)

## 概述

提供一个Arduino库给RTK模块，此库具有以下功能：
  获取 gnss GGA的数据
  配置 CSV文件存储

示例：`configNtrip`、`printGGAData`、`enableSdCSV`。

## 库安装
这里提供两种使用本库的方法：<br>
1.打开Arduino IDE,在状态栏中的Tools--->Manager Libraries 搜索"DFRobot_RTK_LG_EG"并安装本库.<br>
2.首先下载库文件,将其粘贴到\Arduino\libraries目录中,然后打开examples文件夹并在该文件夹中运行演示.<br>

## 方法

```C++
/**
     * @fn begin
     * @brief 初始化
     * @param none
     * @return 0: 成功, 1: 失败
     */
    uint8_t begin(void);

    /**
     * @fn configNtrip
     * @brief 配置NTRIP
     * @param host: 主机地址
     * @param port: 端口号
     * @param user: 用户名
     * @param password: 密码
     * @param mountpoint: 挂载点
     * @return 0: 成功, 1: 失败
     */
    uint8_t configNtrip(String host, String port, String user, String password, String mountpoint);
    
    /**
     * @fn updateGGA
     * @brief 更新GGA数据
     * @param none
     * @return true: 成功, false: 失败
     */
    bool updateGGA(void);

    /**
     * @fn getTime
     * @brief 获取时间
     * @param none
     * @return 时间
     * @note 格式: hhmmss.ss
     */
    String getTime(void);
    /**
     * @fn getLatitude
     * @brief 获取纬度
     * @param none
     * @return 纬度
     * @note 格式: DDMM.MMMM（度和分）
     */
    String getLatitude(void);
    /**
     * @fn getLatitudeDirection
     * @brief 获取纬度方向
     * @param none
     * @return 纬度方向
     * @note N 或 S
     */
    String getLatitudeDirection(void);
    /**
     * @fn getLongitude
     * @brief 获取经度
     * @param none
     * @return 经度
     * @note 格式: DDDMM.MMMM（度和分）
     */
    String getLongitude(void);
    /**
     * @fn getLongitudeDirection
     * @brief 获取经度方向
     * @param none
     * @return 经度方向
     * @note E 或 W
     */
    String getLongitudeDirection(void);
    /**
     * @fn getSatellites
     * @brief 获取卫星数
     * @param none
     * @return 卫星数
     */
    String getSatellites(void);
    /**
     * @fn getAltitude
     * @brief 获取海拔高度
     * @param none
     * @return 海拔高度
     * @note 格式: 海拔高度(米)
     */
    String getAltitude(void);
    /**
     * @fn getGeoidSeparation
     * @brief 获取大地水准面差距
     * @param none
     * @return 大地水准面差距
     * @note 格式: 大地水准面差距(米)
     */
    String getGeoidSeparation(void);
    /**
     * @fn getFixQuality
     * @brief 获取定位质量
     * @param none
     * @return 定位质量
     * @note 常见值: 0 无效, 1 单点, 2 差分, 4 RTK固定, 5 RTK浮动
     */
    String getFixQuality(void);
    /**
     * @fn getDiffAge
     * @brief 获取差分龄期
     * @param none
     * @return 差分龄期
     * @note 格式: 差分龄期(秒)
     */
    String getDiffAge(void);


    /**
     * @fn enableSdCSV
     * @brief 开启将所选 NMEA 语句保存到 SD 卡 CSV，可多选：CSV_CFG_RMC|CSV_CFG_GGA 等
     * @param csv_cfg: 数据类型（eCSVCfg_t 或 多个 eCSVCfg_t 按位或）
     * @return none
     * @note CSV_CFG_ALL 为 0x40，表示全部语句
     */
    void enableSdCSV(uint8_t csv_cfg);

    /**
     * @fn disableSdCSV
     * @brief 停止将所选 NMEA 语句保存到 SD 卡 CSV。先读取设备当前存储掩码，再将传入标志的对应位置 0，可多选：CSV_CFG_RMC|CSV_CFG_GGA 等
     * @param csv_cfg: 数据类型（eCSVCfg_t 或 多个 eCSVCfg_t 按位或）
     * @return 0: 成功, 1: 失败
     */
    uint8_t disableSdCSV(uint8_t csv_cfg);

    /**
     * @fn deleteCSV
     * @brief 按同样的标志删除对应的 CSV 数据，可多选
     * @param csv_cfg: 数据类型（eCSVCfg_t 或 多个按位或）
     * @return none
     */
    void deleteCSV(uint8_t csv_cfg);
```

## 兼容性

MCU                | Work Well    |   Work Wrong    | Untested    | Remarks
------------------ | :----------: | :-------------: | :---------: | :----:
Arduino MEGA2560   |      √       |                 |             |
FireBeetle-ESP32   |      √       |                 |             |


## 历史
- 2026/2/26 - V1.0.0 版本

## 创作者

Written by Jay Tang(jie.tang@dfrobot.com), 2026. (Welcome to our website)
