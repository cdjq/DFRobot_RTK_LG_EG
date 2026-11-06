# DFRobot_RTK_LG_EG
- [English Version](./README.md)
这是一款专为企业级应用设计的高精度GNSS-RTK定位设备，能够实现户外厘米级精度的定位。产品内置4G通信模块，支持通过Ntrip协议快速接入RTK差分服务，也可外接无线数传模块实现高精度定位，灵活适应多种部署环境。
设备集成丰富接口，包括UART、USB等，便于系统集成与扩展。支持上位机快速数据验证，具备掉电数据保存功能，并可通过SD卡扩展存储，确保关键数据不丢失。
核心采用移远LG290P03AAMD模组，支持全星系多频段信号接收，最高输出频率可达20Hz（默认10Hz）。内置干扰检测与抑制算法，即使在电磁环境复杂或城市峡谷等挑战性场景中，依旧能保证定位的准确性。
该产品适用于智能机器人导航、精准农业作业、工业园区智能AGV定位机器人等等，是企业用户快速验证定位功能和集成项目应用的理想选择。

![正反面svg效果图](../../resources/images/TEL0193.png)


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

提供一个python库给模块，此库具有以下功能：
  获取 gnss GGA的数据
  配置 CSV文件存储

示例：`config_ntrip.py`、`print_gga_data.py`、`enable_sd_csv.py`。

## 库安装
1. 下载库至树莓派，要使用这个库，首先要将库下载到Raspberry Pi，命令下载方法如下:

```
sudo git clone https://github.com/DFRobot/DFRobot_RTK_LG_EG
```

1. 打开并运行例程，要执行一个例程demo_x.py，请在命令行中输入python demo_x.py。例如，要执行 print_gga_data.py例程，你需要输入:

```
python  print_gga_data.py
或
python2 print_gga_data.py
或
python3 print_gga_data.py
```

## 方法

```python

    def begin(self):
        '''!
        @brief 初始化并握手
        @return 0: 成功, 1: 失败
        '''

    def config_ntrip(self, host, port, user, password, mountpoint):
        '''!
        @brief 配置 NTRIP：主机、端口、用户名、密码、挂载点
        @param host: 服务器主机地址
        @param port: 端口号（字符串或整数）
        @param user: 用户名
        @param password: 密码
        @param mountpoint: 挂载点
        @return 0: 成功, 1: 失败
        '''

    def update_gga(self):
        '''!
        @brief 请求并更新 GGA 数据
        @return True 表示成功, False 表示失败
        '''

    def get_time(self):
        '''!
        @brief 获取 GGA 的 UTC 时分秒，格式 hhmmss.ss
        @return 时间字符串
        '''

    def get_latitude(self):
        '''!
        @brief 获取纬度，格式 DDMM.MMMM（度和分）
        @return 纬度字符串
        '''

    def get_latitude_direction(self):
        '''!
        @brief 获取纬度方向，N 或 S
        @return 'N' 或 'S' 或 ''
        '''

    def get_longitude(self):
        '''!
        @brief 获取经度，格式 DDDMM.MMMM（度和分）
        @return 经度字符串
        '''

    def get_longitude_direction(self):
        '''!
        @brief 获取经度方向，E 或 W
        @return 'E' 或 'W' 或 ''
        '''

    def get_satellites(self):
        '''!
        @brief 获取 GGA 卫星数字符串
        @return 卫星数量字符串
        '''

    def get_altitude(self):
        '''!
        @brief 获取海拔，单位米
        @return 高度字符串
        '''

    def get_geoid_separation(self):
        '''!
        @brief 获取大地水准面差距，单位米
        @return 分离量字符串
        '''

    def get_fix_quality(self):
        '''!
        @brief 获取定位质量。常见值: 0 无效, 1 单点, 2 差分, 4 RTK固定, 5 RTK浮动
        @return 定位质量字符串
        '''

    def get_diff_age(self):
        '''!
        @brief 获取差分龄期，单位秒
        @return 差分龄期字符串
        '''

    def enable_sd_csv(self, csv_cfg):
        '''!
        @brief 开启将所选 NMEA 语句保存到 SD 卡 CSV（可按位或: CSV_CFG_RMC | CSV_CFG_GGA 等）
        @param csv_cfg: 一个或多个 CSV_CFG_* 标志按位或的结果
        @note CSV_CFG_ALL 为 0x40，表示全部语句
        '''

    def disable_sd_csv(self, csv_cfg):
        '''!
        @brief 停止将所选 NMEA 语句保存到 SD 卡 CSV。先读取设备当前存储掩码，再将传入标志的对应位置 0
        @param csv_cfg: 一个或多个 CSV_CFG_* 标志按位或的结果
        @return 0: 成功, 1: 失败
        '''

    def delete_csv(self, csv_cfg):
        '''!
        @brief 按同样的标志删除对应的 CSV 数据
        @param csv_cfg: 一个或多个 CSV_CFG_* 标志按位或的结果
        '''
```

## 兼容性

* RaspberryPi Version

| Board        | 正常运行  | 运行失败   | 未测试    | 备注
| ------------ | :-------: | :--------: | :------: | :-----: |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python版本

| Python  | 正常运行  | 运行失败   | 未测试    | 备注
| ------- | :-------: | :--------: | :------: | :-----: |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## 历史

- 2026/2/26 - V1.0.0 版本

## 创作者

Written by Jay Tang(jie.tang@dfrobot.com), 2026. (Welcome to our website)
