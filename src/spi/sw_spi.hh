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

#ifndef SW_SPI_HH
#define SW_SPI_HH

#include "spi/spi.hh"

/* -------------------------------------------------------------------------- */
/* Bit banging SPI                                                            */
/* -------------------------------------------------------------------------- */
template<typename _sck, typename _miso, typename _mosi, typename _ss = unused_type,
         bool slave = is_same<_ss, unused_type>::value>
class SWSPI : public SPIInterface< _sck, _miso, _mosi, _ss,
				   SWSPI<_sck, _miso, _mosi, _ss, slave> > {
protected:
  typedef SPIInterface< _sck, _miso, _mosi, _ss,
			SWSPI<_sck, _miso, _mosi, _ss, slave> > interface;
public:
  /* ------------------------------------------------------------------------ */
  static void activate() {
    interface::activate();
    _sck::low();

    bit_order = _spi_bo_lsb;
    mode      = _spi_mode_0;
  }

  /* ------------------------------------------------------------------------ */
  static void unactivate() {
  }

  /* ------------------------------------------------------------------------ */
  static inline void setBitOrder(spi_bit_order_t bo) {
    bit_order = bo;
  }

  /* ------------------------------------------------------------------------ */
  static inline void setMode(spi_mode_t m) {
    mode = m;
    if(mode & 0x2) _sck::high();
  }

  /* ------------------------------------------------------------------------ */
  static inline void setPrescaler(spi_prescaler_t clk) {
  }

  /* ------------------------------------------------------------------------ */
  static uint8_t transferByte(uint8_t sb) {
    uint8_t rb = 0;
    for(uint8_t i = 0; i < 8; ++i) {
      if(mode & 0x01) _sck::write(!_sck::read());

      if (bit_order == _spi_bo_lsb) {
        rb |= _miso::read() << i;
        _mosi::write((sb >> i) & 0x1);
      } else {
        rb |= _miso::read() << (7 - i);
        _mosi::write((sb >> (7 - i)) & 0x01);
      }

      if(!(mode & 0x01)) _sck::write(!_sck::read());
      _sck::write(!_sck::read());
    }

    return rb;
  }

  /* ------------------------------------------------------------------------ */
  static inline void sendByte(uint8_t b) {
    transferByte(b);
  }

  /* ------------------------------------------------------------------------ */
  static inline uint8_t receiveByte(void) {
    uint8_t b(0);
    return transferByte(b);
  }

  /* ------------------------------------------------------------------------ */
  static inline bool isSelected() {
    return (_ss::read() == LOW);
  }

  /* ------------------------------------------------------------------------ */
  static uint8_t available() {
    return 0;
  }

private:
  static spi_bit_order_t bit_order;
  static spi_mode_t      mode;
};

/* -------------------------------------------------------------------------- */
template<typename _sck, typename _miso, typename _mosi, typename _ss, bool slave>
spi_bit_order_t SWSPI<_sck, _miso, _mosi, _ss, slave>::bit_order;

template<typename _sck, typename _miso, typename _mosi, typename _ss, bool slave>
spi_mode_t SWSPI<_sck, _miso, _mosi, _ss, slave>::mode;

#endif // SW_SPI_HH
