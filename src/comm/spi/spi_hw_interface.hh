/**
   This file is part of libavrc++.

   Copyright (c) 2014, netWorms, All rights reserved.

   libavrc++ is  free software: you can  redistribute it and/or modify  it under
   the terms of the  GNU Lesser General Public License as  published by the Free
   Software Foundation, either version 3 of the License, or (at your option) any
   later version.

   libavrc++ is distributed in the hope that  it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR  PURPOSE.  See the  GNU Lesser  General Public License  for more
   details.

   You should  have received  a copy  of the GNU  Lesser General  Public License
   along with libavrc++.  If not, see <http://www.gnu.org/licenses/>.
*/

/* -------------------------------------------------------------------------- */
#include "spi/spi_interface.hh"
#include "common/queue.hh"
/* -------------------------------------------------------------------------- */

#ifndef SPI_HW_INTERFACE_HH
#define SPI_HW_INTERFACE_HH

#define SPI_SLAVE_NOT_IMPLEMENTED

#if (RAMEND < 1000)
#define SPI_BUFFER_SIZE 32
#else
#define SPI_BUFFER_SIZE 64
#endif

namespace spi {
enum _prescaler_t {
  _clk_2 = 0x04,
  _clk_4 = 0x00,
  _clk_8 = 0x05,
  _clk_16 = 0x01,
  _clk_32 = 0x06,
  _clk_64 = 0x02, // 0x07
  _clk_128 = 0x03
};

/* -------------------------------------------------------------------------- */
/* SPI interface                                                              */
/* -------------------------------------------------------------------------- */
template <typename _sck, typename _miso, typename _mosi, typename _ss>
class SPIInterface<_sck, _miso, _mosi, _ss, _hw_spi>
    : public Communication<SPIInterface<_sck, _miso, _mosi, _ss, _hw_spi>> {
  typedef spcr control_reg;
  typedef spsr status_reg;
  typedef spdr data_reg;
public:
  /* ------------------------------------------------------------------------ */
  static void activate() {
    /* Enable SPI, Master, set clock rate fck/16 */
    control_reg::sbit(MSTR);
    control_reg::sbit(SPR0);

#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
    /* Enable Interrupt */
    if (slave)
      status_reg::sbit(SPIE);
#endif
    /* Enable SPI */
    control_reg::sbit(SPE);

    mode = spi::_mode_0;
    bit_order = spi::_bo_msb;
  }

  static void select() {
    setMode(mode);
    setBitOrder(bit_order);
  }

  /* ------------------------------------------------------------------------ */
  static void unactivate() {
    /* disable SPI */
    control_reg::cbit(SPE);
  }

  /* ------------------------------------------------------------------------ */
  static inline void setBitOrder(_bit_order_t bo) {
    bit_order = bo;
    control_reg::template sbits<DORD, 0x01>(bo);
  }

  /* ------------------------------------------------------------------------ */
  static inline void setMode(_mode_t _mode) {
    mode = _mode;
    control_reg::template sbits<CPHA, 0x03>(mode);
  }

  /* ------------------------------------------------------------------------ */
  static inline void setPrescaler(_prescaler_t clk) {
    control_reg::template sbits<SPR0, 0x03>(clk);
    status_reg::template sbits<SPI2X, 0x01>(clk >> 2);
  }

  /* ------------------------------------------------------------------------ */
  static uint8_t transferByte(uint8_t b) {
    data_reg::set(b);
    status_reg::wait_bit_is_set(SPIF);
    return data_reg::val();
  }

  /* ------------------------------------------------------------------------ */
  static inline void sendByte(uint8_t b) {
#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
    if (slave) {
      send_buffer.push(b);
    } else {
#endif
      receive_buffer.push(transferByte(b));
#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
    }
#endif
  }

  /* ------------------------------------------------------------------------ */
  static inline uint8_t receiveByte(void) { return receive_buffer.pop(); }

  /* ------------------------------------------------------------------------ */
  static inline bool isSelected() { return (_ss::read() == LOW); }

  /* ------------------------------------------------------------------------ */
  static inline void interruptTransfer() {
    uint8_t b = data_reg::val();

    receive_buffer.push(b);
#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
    if (!send_buffer.isEmpty())
      data_reg::set(send_buffer.pop());
#endif
  }

  /* ------------------------------------------------------------------------ */
  static uint8_t available() { return receive_buffer.used(); }

private:
  static _mode_t mode;
  static _bit_order_t bit_order;

  static Queue<uint8_t, uint8_t, SPI_BUFFER_SIZE> receive_buffer;
#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
  static Queue<uint8_t, uint8_t, SPI_BUFFER_SIZE> send_buffer;
#endif
};

/* -------------------------------------------------------------------------- */
template <typename _sck, typename _miso, typename _mosi, typename _ss>
spi::_mode_t SPIInterface<_sck, _miso, _mosi, _ss, spi::_hw_spi>::mode;

template <typename _sck, typename _miso, typename _mosi, typename _ss>
spi::_bit_order_t SPIInterface<_sck, _miso, _mosi, _ss, spi::_hw_spi>::bit_order;

template <typename _sck, typename _miso, typename _mosi, typename _ss>
Queue<uint8_t, uint8_t, SPI_BUFFER_SIZE>
    SPIInterface<_sck, _miso, _mosi, _ss, spi::_hw_spi>::receive_buffer;

#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
template <typename _sck, typename _miso, typename _mosi, typename _ss>
Queue<uint8_t, uint8_t, SPI_BUFFER_SIZE>
    SPIInterface<_sck, _miso, _mosi, _ss, spi::_hw_spi>::send_buffer;
#endif
} // spi
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#endif /* SPI_HW_INTERFACE_HH */
