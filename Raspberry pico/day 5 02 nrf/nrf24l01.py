# nrf24l01.py - MicroPython NRF24L01 driver
import utime
from micropython import const

NRF24L01_CONFIG = const(0x00)
EN_AA = const(0x01)
EN_RXADDR = const(0x02)
SETUP_AW = const(0x03)
SETUP_RETR = const(0x04)
RF_CH = const(0x05)
RF_SETUP = const(0x06)
STATUS = const(0x07)
RX_ADDR_P0 = const(0x0a)
TX_ADDR = const(0x10)
RX_PW_P0 = const(0x11)
FIFO_STATUS = const(0x17)
DYNPD = const(0x1c)

POWER_DOWN = const(0x00)
POWER_UP = const(0x02)
PTX = const(0x00)
PRX = const(0x01)

CONFIG = const(0x0a)

R_RX_PL_WID = const(0x60)
R_RX_PAYLOAD = const(0x61)
W_TX_PAYLOAD = const(0xa0)
FLUSH_TX = const(0xe1)
FLUSH_RX = const(0xe2)
NOP = const(0xff)

RX_DR = const(0x40)
TX_DS = const(0x20)
MAX_RT = const(0x10)


class NRF24L01:
    def __init__(self, spi, cs, ce, channel=76, payload_size=16):
        assert payload_size <= 32
        self.buf = bytearray(1)
        self.spi = spi
        self.cs = cs
        self.ce = ce
        self.payload_size = payload_size
        self.pipe0_read_addr = None
        self.cs.init(self.cs.OUT, value=1)
        self.ce.init(self.ce.OUT, value=0)
        utime.sleep_ms(5)

        self.reg_write(SETUP_AW, 0b11)
        self.reg_write(RF_CH, min(channel, 125))
        self.reg_write(RF_SETUP, 0x0f)
        self.reg_write(NRF24L01_CONFIG, 0x0c)
        self.set_power_speed(0x0f)
        self.set_crc(2)
        self.set_retries(5, 15)

        self.reg_write(DYNPD, 0)
        self.reg_write(EN_RXADDR, 0)
        self.flush_rx()
        self.flush_tx()

    def reg_write(self, reg, value):
        self.cs(0)
        self.spi.readinto(self.buf, 0x20 | reg)
        self.spi.write(bytes([value]))
        self.cs(1)

    def reg_write_bytes(self, reg, buf):
        self.cs(0)
        self.spi.readinto(self.buf, 0x20 | reg)
        self.spi.write(buf)
        self.cs(1)

    def reg_read(self, reg):
        self.cs(0)
        self.spi.readinto(self.buf, reg)
        buf = self.spi.read(1)
        self.cs(1)
        return buf[0]

    def reg_read_bytes(self, reg, nbytes=5):
        self.cs(0)
        self.spi.readinto(self.buf, reg)
        buf = self.spi.read(nbytes)
        self.cs(1)
        return buf

    def flush_rx(self):
        self.cs(0)
        self.spi.readinto(self.buf, FLUSH_RX)
        self.cs(1)

    def flush_tx(self):
        self.cs(0)
        self.spi.readinto(self.buf, FLUSH_TX)
        self.cs(1)

    def set_power_speed(self, power_speed):
        self.reg_write(RF_SETUP, power_speed)

    def set_crc(self, length):
        config = self.reg_read(NRF24L01_CONFIG) & ~0x0c
        if length == 0:
            pass
        elif length == 1:
            config |= 0x08
        else:
            config |= 0x0c
        self.reg_write(NRF24L01_CONFIG, config)

    def set_retries(self, delay, count):
        self.reg_write(SETUP_RETR, (delay & 0xf) << 4 | (count & 0xf))

    def open_tx_pipe(self, address):
        assert len(address) == 5
        self.reg_write_bytes(RX_ADDR_P0, address)
        self.reg_write_bytes(TX_ADDR, address)
        self.reg_write(RX_PW_P0, self.payload_size)

    def open_rx_pipe(self, pipe_id, address):
        assert len(address) == 5
        assert 0 <= pipe_id <= 5
        if pipe_id == 0:
            self.pipe0_read_addr = address
        if pipe_id < 2:
            self.reg_write_bytes(RX_ADDR_P0 + pipe_id, address)
        else:
            self.reg_write_bytes(RX_ADDR_P0 + pipe_id, address[:1])
        self.reg_write(RX_PW_P0 + pipe_id, self.payload_size)
        self.reg_write(EN_RXADDR, self.reg_read(EN_RXADDR) | (1 << pipe_id))

    def start_listening(self):
        self.reg_write(NRF24L01_CONFIG, self.reg_read(NRF24L01_CONFIG) | PRX | POWER_UP)
        self.reg_write(STATUS, RX_DR | TX_DS | MAX_RT)
        if self.pipe0_read_addr is not None:
            self.reg_write_bytes(RX_ADDR_P0, self.pipe0_read_addr)
        self.flush_rx()
        self.flush_tx()
        self.ce(1)
        utime.sleep_us(130)

    def stop_listening(self):
        self.ce(0)
        self.flush_tx()
        self.flush_rx()

    def any(self):
        return not bool(self.reg_read(FIFO_STATUS) & 0x01)

    def recv(self):
        buf = self.reg_read_bytes(R_RX_PAYLOAD, self.payload_size)
        self.reg_write(STATUS, RX_DR)
        return buf

    def send_start(self, buf):
        self.reg_write(STATUS, RX_DR | TX_DS | MAX_RT)
        if len(buf) < self.payload_size:
            buf += b'\x00' * (self.payload_size - len(buf))
        self.cs(0)
        self.spi.readinto(self.buf, W_TX_PAYLOAD)
        self.spi.write(buf)
        self.cs(1)
        self.ce(1)

    def send_done(self):
        if not (self.reg_read(STATUS) & (TX_DS | MAX_RT)):
            return None
        result = self.reg_read(STATUS) & TX_DS > 0
        self.ce(0)
        self.reg_write(STATUS, RX_DR | TX_DS | MAX_RT)
        self.flush_tx()
        return result

    def send(self, buf, timeout=500):
        self.send_start(buf)
        start = utime.ticks_ms()
        result = None
        while result is None and utime.ticks_diff(utime.ticks_ms(), start) < timeout:
            result = self.send_done()
        self.ce(0)
        if result is None:
            raise OSError("send timeout")
        if not result:
            raise OSError("send failed")