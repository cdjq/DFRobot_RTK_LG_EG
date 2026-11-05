# DFRobot_RTK_LG_EG 注释、命名与示例整理

日期：2026-11-05

发布前整理。直接替换不准确的公开名字，不保留旧名字。只改正文写错或看不懂的注释。示例拆成三个，Arduino 与 Python 同步。协议、返回值和源文件排版不变。版本保持 1.0.0。

## 公开接口

只改 CSV 这组对外名字。命令字节和参数宽度不变。

| 现在 | 改成 | 行为 |
| --- | --- | --- |
| `CSVSlaveCfg(uint8_t slave_cfg)` | `configCSV(uint8_t csv_cfg)` | 仍发送 `CMD_CONFIG_CSV`（`0xA4`），参数仍是 1 字节标志 |
| `CSVDelCfg(uint8_t del_cfg)` | `deleteCSV(uint8_t csv_cfg)` | 仍发送 `CMD_CONFIG_DEL_CSV`（`0xA5`） |
| `SLAVE_CFG_RMC` | `CSV_CFG_RMC` | `0x01` |
| `SLAVE_CFG_GGA` | `CSV_CFG_GGA` | `0x02` |
| `SLAVE_CFG_GSV` | `CSV_CFG_GSV` | `0x04` |
| `SLAVE_CFG_GSA` | `CSV_CFG_GSA` | `0x08` |
| `SLAVE_CFG_VTG` | `CSV_CFG_VTG` | `0x10` |
| `SLAVE_CFG_GLL` | `CSV_CFG_GLL` | `0x20` |
| `SLAVE_CFG_ALL` | `CSV_CFG_ALL` | `0x40` |

枚举类型名 `eCSVCfg_t` 保持不变。`CSV_CFG_ALL` 是单独的 `0x40`，表示全部语句。它不是其余标志的按位或；其余标志的或是 `0x3F`。多个类型继续按位或，例如 `CSV_CFG_RMC | CSV_CFG_GGA`。

Python 对应关系：

- `csv_slave_cfg` → `config_csv`
- `csv_del_cfg` → `delete_csv`
- `SLAVE_CFG_*` → `CSV_CFG_*`，数值与上表相同

以下名字和参数保持不变：`begin`、`configNtrip`、`updateGGA`、`getTime`、`getLatitude`、`getLatitudeDirection`、`getLongitude`、`getLongitudeDirection`、`getSatellites`、`getAltitude`、`getGeoidSeparation`、`getFixQuality`、`getDiffAge`。Python 侧保持 `begin`、`config_ntrip`、`update_gga` 和现有 `get_*`。

`keywords.txt`、根目录 `README.md`、`README_CN.md`、`python/raspberrypi/README.md`、`python/raspberrypi/README_CN.md` 中的上述符号换成新名字。

不改：`VOREO_REQUEST_TIMEOUT`、命令宏、帧格式、`.cpp` 收发逻辑、私有函数。

## 注释

Doxygen 标签顺序、缩进、空行和中英文分工保持原样。`.h` 继续英文。`.cpp` 里已经正确的中文协议注释保留。README 仍是中英各一份。

头文件（含 `sGGAData_t` 各字段的 `///<` 注释）、两份根 README，以及 `python/raspberrypi/README.md` 使用下面的含义：

- `updateGGA` 的 `@fn` 写 `updateGGA`。中文 README 里现有的 `@fn update` 改为 `updateGGA`。
- `getTime`：GGA 的 UTC 时分秒，格式 `hhmmss.ss`。
- `getLatitude`：纬度，格式 `DDMM.MMMM`（度和分）。中文不再写成度分秒。
- `getLongitude`：经度，格式 `DDDMM.MMMM`（度和分）。
- `getLatitudeDirection` / `getLongitudeDirection`：分别为 `N`/`S`、`E`/`W`。
- `getSatellites`：GGA 里的卫星数字符串。删除从 0 颗写到 20 颗的说明。
- `getAltitude`：海拔，单位米。
- `getGeoidSeparation`：大地水准面差距，单位米。
- `getFixQuality`：NMEA GGA 定位质量字符串。常见值：`0` 无效，`1` 单点，`2` 差分，`4` RTK 固定，`5` RTK 浮动。
- `getDiffAge`：差分龄期，单位秒。
- `configCSV`：按标志选择要写入 CSV 的 NMEA 语句，多个标志按位或。
- `deleteCSV`：按同样的标志删除对应的 CSV 数据。
- `CSV_CFG_ALL` 注明它是单独的 `0x40`，表示全部语句。

`.cpp` 只改两处：CRC 注释去掉对仓库中不存在的 `text.py` 的引用，写成 CRC-16/IBM（初值 `0xFFFF`，多项式 `0xA001`）；`CSVSlaveCfg` / `CSVDelCfg` 的实现改为 `configCSV` / `deleteCSV`。帧格式和 GGA 字段顺序注释保留。

Python 文档字符串使用同一套事实，函数名改为 `config_csv` / `delete_csv`。`python/raspberrypi/README_CN.md` 同步这些句子。中文根 README 目录锚点 `兼容性y` 改为 `兼容性`。`configNtrip` 参数列表里多出来的 `*` 去掉。

## 示例

Arduino 与 Python 各三个示例，每个文件只做一件事。接线表、文件头和缩进沿用 `examples/printGGAData/printGGAData.ino` 与 `python/raspberrypi/examples/printGGAData.py` 的现有写法。删除 `python/raspberrypi/examples/print_gga_data.py`。

### configNtrip

路径：

- `examples/configNtrip/configNtrip.ino`
- `python/raspberrypi/examples/configNtrip.py`

`begin` 成功后调用 `configNtrip` / `config_ntrip`。参数继续使用现有账号：主机 `103.45.160.41`，端口 `8002`，用户 `maplays33280`，密码 `66794`，挂载点 `RTCM32GRCpro`。`loop` 只延时，不读取定位。

### printGGAData

路径保持：

- `examples/printGGAData/printGGAData.ino`
- `python/raspberrypi/examples/printGGAData.py`

去掉其中的 NTRIP 配置调用。文件头注释说明：厘米级 RTK 需要先运行 `configNtrip`，或使用模块自己的数传链路。`loop` 调用 `updateGGA()` / `update_gga()`。失败时打印失败并跳过本次。成功时打印现有全部字段。时间、纬度、经度、定位质量旁边注明 `hhmmss.ss`、`DDMM.MMMM`、`DDDMM.MMMM`，以及 `0`/`1`/`2`/`4`/`5` 的含义。

### configCSV

路径保持：

- `examples/configCSV/configCSV.ino`
- `python/raspberrypi/examples/configCSV.py`

`begin` 成功后调用 `configCSV(CSV_CFG_RMC | CSV_CFG_GGA)` / `config_csv(CSV_CFG_RMC | CSV_CFG_GGA)`。注释列出 `CSV_CFG_RMC`、`CSV_CFG_GGA`、`CSV_CFG_GSV`、`CSV_CFG_GSA`、`CSV_CFG_VTG`、`CSV_CFG_GLL`、`CSV_CFG_ALL` 的含义，并写明 `CSV_CFG_ALL` 是 `0x40`。`deleteCSV(CSV_CFG_RMC | CSV_CFG_GGA)` / `delete_csv(...)` 只出现在注释里，默认不执行。`loop` 只延时。

根目录和 `python/raspberrypi` 的 README 概述写明这三个示例的名字。安装说明里的运行例子仍以 `printGGAData.py` 为例。

## 范围与验证

保持不变：

- 帧解析、CRC 算法、波特率、引脚、`VOREO_REQUEST_TIMEOUT`、私有函数、命令字节
- `begin` 与 `updateGGA` 的返回值
- `configNtrip`、`configCSV`、`deleteCSV` 的 `void` 返回
- `.h` 与 `.cpp` 的缩进和空行。示例按 `printGGAData` 的现有版式重写内容，不另起一套格式
- `library.properties` 版本 `1.0.0` 和 History

示例错误处理：

- `begin` 失败时保持现在的每秒重试
- `printGGAData` 在 `updateGGA()` / `update_gga()` 失败时打印一行失败信息并跳过本次打印

验证：

- 检索确认 `CSVSlaveCfg`、`CSVDelCfg`、`SLAVE_CFG_`、`csv_slave_cfg`、`csv_del_cfg` 和 `print_gga_data.py` 不再存在
- 对照头文件检查 README 与 Python 文档字符串
- 用 Python 编译检查 `python/raspberrypi/DFRobot_RTK_LG_EG.py` 和三个 Python 示例，确认语法通过
- Arduino 示例与头文件、README 人工对照。此环境不编译固件，也不在设备上跑定位
