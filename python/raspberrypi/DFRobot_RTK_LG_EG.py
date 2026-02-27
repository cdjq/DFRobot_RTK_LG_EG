# -*- coding: utf-8 -*-
'''!
@file DFRobot_RTK_LG_EG.py
@brief DFRobot RTK LG/EG library for Raspberry Pi, UART protocol (55 AA len cmd payload crc 0D)
@copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@license The MIT License (MIT)
@version V1.0
@url https://github.com/DFRobot/DFRobot_RTK_LG_EG
'''

import serial
import time

# Python 2/3 serial buffer check
def _in_waiting(ser):
    return ser.in_waiting if hasattr(ser, 'in_waiting') else ser.inWaiting()

# Frame protocol (same as C++ / text.py)
FRAME_HEAD1 = 0x55
FRAME_HEAD2 = 0xAA
FRAME_TAIL = 0x0D
FRAME_MAX_LEN = 256

# Commands
CMD_HANDSHAKE = 0xA0
CMD_CONFIG_GNSS = 0xA1
CMD_CONFIG_4G = 0xA2
CMD_CONFIG_NTRIP = 0xA3
CMD_CONFIG_CSV = 0xA4
CMD_CONFIG_DEL_CSV = 0xA5
CMD_CONFIG_UART_OUT = 0xA6
CMD_GET_GGA = 0xC1
BAUD_RATE_115200 = 115200
VOREO_REQUEST_TIMEOUT = 10.0  # seconds

# CSV config flags (can be ORed)
SLAVE_CFG_RMC = 0x01
SLAVE_CFG_GGA = 0x02
SLAVE_CFG_GSV = 0x04
SLAVE_CFG_GSA = 0x08
SLAVE_CFG_VTG = 0x10
SLAVE_CFG_GLL = 0x20
SLAVE_CFG_ALL = 0x40


def _crc16_ibm(data):
    '''CRC-16 IBM: init 0xFFFF, poly 0xA001, LSB-first (same as C++ / text.py).'''
    crc = 0xFFFF
    for b in data:
        if isinstance(b, str):
            b = ord(b)
        crc ^= b
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
            crc &= 0xFFFF
    return crc


def _build_frame(cmd, payload=b''):
    '''Build one frame: 55 AA len cmd payload crc_lo crc_hi 0D. payload max 255 bytes.'''
    if payload is None:
        payload = b''
    payload = bytes(payload)
    if len(payload) > 255:
        raise ValueError('payload too long (max 255)')
    crc_input = bytes([cmd]) + payload
    crc = _crc16_ibm(crc_input)
    crc_lo = crc & 0xFF
    crc_hi = (crc >> 8) & 0xFF
    return bytes([FRAME_HEAD1, FRAME_HEAD2, len(payload), cmd]) + payload + bytes([crc_lo, crc_hi, FRAME_TAIL])


class DFRobot_RTK_LG_EG(object):
    '''!
    RTK LG/EG GPS library, UART only. Protocol: 55 AA len cmd payload crc_lo crc_hi 0D, CRC-16 IBM.
    '''

    def __init__(self, port='/dev/ttyAMA0', baud=BAUD_RATE_115200):
        '''!
        @brief Constructor
        @param port: serial port path, e.g. /dev/ttyAMA0, /dev/ttyS0, /dev/ttyUSB0
        @param baud: baud rate, default 115200
        '''
        self._ser = serial.Serial(port, baudrate=baud, timeout=0.1)
        try:
            is_open = self._ser.is_open
        except AttributeError:
            is_open = self._ser.isOpen()
        if not is_open:
            self._ser.open()
        self._gga_data = {
            'UTC': '',
            'Latitude': '',
            'LatitudeDirection': '',
            'Longitude': '',
            'LongitudeDirection': '',
            'Satellites': '',
            'Altitude': '',
            'GeoidSeparation': '',
            'FixQuality': '',
            'DiffAge': '',
        }

    def _send_command(self, cmd, data=None):
        '''Send one frame. data: bytes or list of int, or None for empty payload.'''
        payload = b'' if data is None else (bytes(data) if isinstance(data, (list, bytes, bytearray)) else data)
        frame = _build_frame(cmd, payload)
        self._ser.write(frame)

    def _receive_response(self, cmd):
        '''!
        Receive one frame, expect given cmd. Return (payload_len, payload_bytes) or (-1, None) on failure.
        '''
        timeout = VOREO_REQUEST_TIMEOUT
        start = time.time()
        # Sync to 55 AA
        while True:
            if time.time() - start >= timeout:
                return -1, None
            if _in_waiting(self._ser) < 2:
                time.sleep(0.001)
                continue
            b0 = self._ser.read(1)
            if not b0 or b0[0] != FRAME_HEAD1:
                continue
            if _in_waiting(self._ser) < 1:
                if time.time() - start >= timeout:
                    return -1, None
                continue
            b1 = self._ser.read(1)
            if b1[0] == FRAME_HEAD2:
                break
        # Read len, cmd
        while _in_waiting(self._ser) < 2:
            if time.time() - start >= timeout:
                return -1, None
            time.sleep(0.001)
        buf = self._ser.read(2)
        length, cmd_recv = buf[0], buf[1]
        if cmd_recv != cmd:
            return -1, None
        if length > 255:
            return -1, None
        # Read payload
        payload = b''
        while len(payload) < length:
            if time.time() - start >= timeout:
                return -1, None
            n = self._ser.read(length - len(payload))
            payload += n
            if not n:
                time.sleep(0.001)
        # Read crc_lo, crc_hi, tail
        while _in_waiting(self._ser) < 3:
            if time.time() - start >= timeout:
                return -1, None
            time.sleep(0.001)
        tail_buf = self._ser.read(3)
        crc_lo, crc_hi, tail = tail_buf[0], tail_buf[1], tail_buf[2]
        if tail != FRAME_TAIL:
            return -1, None
        crc_recv = crc_lo | (crc_hi << 8)
        crc_input = bytes([cmd_recv]) + payload
        crc_calc = _crc16_ibm(crc_input)
        if crc_calc != crc_recv:
            return -1, None
        return length, payload

    def begin(self):
        '''!
        @brief Init and handshake
        @return 0: success, 1: fail
        '''
        self._send_command(CMD_HANDSHAKE, None)
        length, data = self._receive_response(CMD_HANDSHAKE)
        if length <= 0 or data is None:
            return 1
        if len(data) > 0 and data[0] != 0x01:
            return 1
        return 0

    def config_ntrip(self, host, port, user, password, mountpoint):
        '''!
        @brief Config NTRIP: host, port, user, password, mountpoint
        @param host: server host
        @param port: port string or int
        @param user: user name
        @param password: password
        @param mountpoint: mount point
        '''
        s = '%s,%s,%s,%s,%s' % (host, str(port), user, password, mountpoint)
        self._send_command(CMD_CONFIG_NTRIP, s.encode('utf-8'))

    def _parse_gga(self, payload):
        '''Parse NMEA GGA comma-separated payload into self._gga_data.'''
        try:
            text = payload.decode('utf-8', errors='ignore')
        except Exception:
            text = ''.join(chr(b) if b < 256 else '?' for b in payload)
        parts = text.split(',')
        if len(parts) < 14:
            return
        self._gga_data['UTC'] = parts[1] if len(parts) > 1 else ''
        self._gga_data['Latitude'] = parts[2] if len(parts) > 2 else ''
        self._gga_data['LatitudeDirection'] = parts[3] if len(parts) > 3 else ''
        self._gga_data['Longitude'] = parts[4] if len(parts) > 4 else ''
        self._gga_data['LongitudeDirection'] = parts[5] if len(parts) > 5 else ''
        self._gga_data['FixQuality'] = parts[6] if len(parts) > 6 else ''
        self._gga_data['Satellites'] = parts[7] if len(parts) > 7 else ''
        self._gga_data['Altitude'] = parts[9] if len(parts) > 9 else ''
        self._gga_data['GeoidSeparation'] = parts[11] if len(parts) > 11 else ''
        self._gga_data['DiffAge'] = parts[13] if len(parts) > 13 else ''

    def update_gga(self):
        '''!
        @brief Request and update GGA data
        @return True on success, False on fail
        '''
        self._send_command(CMD_GET_GGA, None)
        length, data = self._receive_response(CMD_GET_GGA)
        if length <= 0 or data is None:
            return False
        self._parse_gga(data)
        return True

    def get_time(self):
        '''!
        @brief Get UTC time string (GGA field)
        @return time string
        '''
        return self._gga_data['UTC'] or ''

    def get_latitude(self):
        '''!
        @brief Get latitude (GGA field)
        @return latitude string
        '''
        return self._gga_data['Latitude'] or ''

    def get_latitude_direction(self):
        '''!
        @brief Get latitude direction N/S
        @return 'N' or 'S' or ''
        '''
        return self._gga_data['LatitudeDirection'] or ''

    def get_longitude(self):
        '''!
        @brief Get longitude (GGA field)
        @return longitude string
        '''
        return self._gga_data['Longitude'] or ''

    def get_longitude_direction(self):
        '''!
        @brief Get longitude direction E/W
        @return 'E' or 'W' or ''
        '''
        return self._gga_data['LongitudeDirection'] or ''

    def get_satellites(self):
        '''!
        @brief Get satellite count (GGA field)
        @return satellite count string
        '''
        return self._gga_data['Satellites'] or ''

    def get_altitude(self):
        '''!
        @brief Get altitude in meters (GGA field)
        @return altitude string
        '''
        return self._gga_data['Altitude'] or ''

    def get_geoid_separation(self):
        '''!
        @brief Get geoid separation (GGA field)
        @return geoid separation string
        '''
        return self._gga_data['GeoidSeparation'] or ''

    def get_fix_quality(self):
        '''!
        @brief Get fix quality (GGA field)
        @return fix quality string
        '''
        return self._gga_data['FixQuality'] or ''

    def get_diff_age(self):
        '''!
        @brief Get differential age (GGA field)
        @return diff age string
        '''
        return self._gga_data['DiffAge'] or ''

    def csv_slave_cfg(self, slave_cfg):
        '''!
        @brief Config CSV data types (can OR: SLAVE_CFG_RMC | SLAVE_CFG_GGA)
        @param slave_cfg: one or more SLAVE_CFG_* flags ORed
        '''
        self._send_command(CMD_CONFIG_CSV, [slave_cfg & 0xFF])

    def csv_del_cfg(self, del_cfg):
        '''!
        @brief Delete CSV data types (can OR multiple flags)
        @param del_cfg: one or more SLAVE_CFG_* flags ORed
        '''
        self._send_command(CMD_CONFIG_DEL_CSV, [del_cfg & 0xFF])
