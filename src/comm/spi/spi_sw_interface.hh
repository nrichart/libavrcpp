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
/* -------------------------------------------------------------------------- */

#ifndef SPI_SW_INTERFACE_HH
#define SPI_SW_INTERFACE_HH

namespace spi {

/* -------------------------------------------------------------------------- */
/* Bit banging SPI                                                            */
/* -------------------------------------------------------------------------- */
template <typename _sck, typename _miso, typename _mosi, typename _ss>
class SPIInterface<_sck, _miso, _mosi, _ss, _sw_spi>
    : public Communication<SPIInterface<_sck, _miso, _mosi, _ss, _sw_spi>> {
public:
  /* ------------------------------------------------------------------------ */
  static void activate() {
    _sck::low();

    bit_order = _bo_lsb;
    mode = _mode_0;
  }

  /* ------------------------------------------------------------------------ */
  static void unactivate() {}

  /* ------------------------------------------------------------------------ */
  static inline void setBitOrder(_bit_order_t bo) { bit_order = bo; }

  /* ------------------------------------------------------------------------ */
  static inline void setMode(_mode_t m) {
    mode = m;
    if (mode & 0x2)
      _sck::high();
  }

  /* ------------------------------------------------------------------------ */
  static inline void setPrescaler(_prescaler_t clk) {}

  /* ------------------------------------------------------------------------ */
  static uint8_t transferByte(uint8_t sb) {
    uint8_t rb = 0;
    for (uint8_t i = 0; i < 8; ++i) {
      if (mode & 0x01)
        _sck::write(!_sck::read());

      if (bit_order == _bo_lsb) {
        rb |= _miso::read() << i;
        _mosi::write((sb >> i) & 0x1);
      } else {
        rb |= _miso::read() << (7 - i);
        _mosi::write((sb >> (7 - i)) & 0x01);
      }

      if (!(mode & 0x01))
        _sck::write(!_sck::read());
      _sck::write(!_sck::read());
    }

    return rb;
  }

  /* ------------------------------------------------------------------------ */
  static inline void sendByte(uint8_t b) { transferByte(b); }

  /* ------------------------------------------------------------------------ */
  static inline uint8_t receiveByte(void) {
    uint8_t b(0);
    return transferByte(b);
  }

  /* ------------------------------------------------------------------------ */
  static inline bool isSelected() { return (_ss::read() == LOW); }

  /* ------------------------------------------------------------------------ */
  static uint8_t available() { return 0; }

private:
  static _bit_order_t bit_order;
  static _mode_t mode;
};

/* -------------------------------------------------------------------------- */
template <typename _sck, typename _miso, typename _mosi, typename _ss>
_bit_order_t SPIInterface<_sck, _miso, _mosi, _ss, _sw_spi>::bit_order;

template <typename _sck, typename _miso, typename _mosi, typename _ss>
_mode_t SPIInterface<_sck, _miso, _mosi, _ss, _sw_spi>::mode;

} // spi

#endif // SW_SPI_HH
