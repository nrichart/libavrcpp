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

#ifndef HWSPI_HH
#define HWSPI_HH

#include "common/queue.hh"
#include "common/communication.hh"
#include "spi/spi.hh"

#define SPI_SLAVE_NOT_IMPLEMENTED

#if (RAMEND < 1000)
  #define SPI_BUFFER_SIZE 32
#else
  #define SPI_BUFFER_SIZE 64
#endif

enum spi_bit_order_t {
  _spi_bo_msb = 0,
  _spi_bo_lsb = 1
};

enum spi_prescaler_t {
  _spi_clk_2   = 0x04,
  _spi_clk_4   = 0x00,
  _spi_clk_8   = 0x05,
  _spi_clk_16  = 0x01,
  _spi_clk_32  = 0x06,
  _spi_clk_64  = 0x02, // 0x07
  _spi_clk_128 = 0x03
};

enum spi_clock_polarity_t {
  _spi_cpol_rising  = 0,
  _spi_cpol_falling = 1
};

enum spi_clock_phase_t {
  _spi_cpha_sample = 0,
  _spi_cpha_setup  = 1
};

enum spi_mode_t {
  _spi_mode_0 = (_spi_cpol_rising  << 1) | _spi_cpha_sample,
  _spi_mode_1 = (_spi_cpol_rising  << 1) | _spi_cpha_setup,
  _spi_mode_2 = (_spi_cpol_falling << 1) | _spi_cpha_sample,
  _spi_mode_3 = (_spi_cpol_falling << 1) | _spi_cpha_setup
};

extern uint8_t hw_spi_activated;
/* -------------------------------------------------------------------------- */
/* SPI interface                                                              */
/* -------------------------------------------------------------------------- */
template<typename _sck, typename _miso, typename _mosi, typename _ss,
         bool slave = is_same<_ss, unused_type>::value,
         typename control_reg = spcr, typename status_reg = spsr, typename data_reg = spdr>
class HWSPI : public SPIInterface< _sck, _miso, _mosi, _ss,
				   HWSPI<_sck, _miso, _mosi, _ss, slave,
					 control_reg, status_reg, data_reg> > {
  typedef SPIInterface< _sck, _miso, _mosi, _ss,
			HWSPI<_sck, _miso, _mosi, _ss, slave,
			      control_reg, status_reg, data_reg> > interface;

public:

  /* ------------------------------------------------------------------------ */
  static void activate() {
    interface::activate();


    /* Enable SPI, Master, set clock rate fck/16 */
    control_reg::sbit(MSTR);
    control_reg::sbit(SPR0);

    /* Enable Interrupt */
    if(slave)
      status_reg::sbit(SPIE);

    /* Enable SPI */
    control_reg::sbit(SPE);

    mode      = _spi_mode_0;
    bit_order = _spi_bo_msb;
    hw_spi_activated++;
  }

  static void select() {
    setMode(mode);
    setBitOrder(bit_order);
    interface::select();
  }

  /* ------------------------------------------------------------------------ */
  static void unactivate() {
    /* disable SPI */
    control_reg::cbit(SPE);
  }

  /* ------------------------------------------------------------------------ */
  static inline void setBitOrder(spi_bit_order_t bo) {
    bit_order = bo;
    control_reg::template sbits<DORD, 0x01>(bo);
  }

  /* ------------------------------------------------------------------------ */
  static inline void setMode(spi_mode_t _mode) {
    mode = _mode;
    control_reg::template sbits<CPHA, 0x03>(mode);
  }

  /* ------------------------------------------------------------------------ */
  static inline void setPrescaler(spi_prescaler_t clk) {
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
    if(slave) {
#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
      send_buffer.push(b);
#endif
    } else {
      receive_buffer.push(transferByte(b));
    }
  }

  /* ------------------------------------------------------------------------ */
  static inline uint8_t receiveByte(void) {
    return receive_buffer.pop();
  }

  /* ------------------------------------------------------------------------ */
  static inline bool isSelected() {
    return (_ss::read() == LOW);
  }

  /* ------------------------------------------------------------------------ */
  static inline void interruptTransfer() {
    uint8_t b = data_reg::val();

    receive_buffer.push(b);
#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
    if(!send_buffer.isEmpty())
      data_reg::set(send_buffer.pop());
#endif
  }

  /* ------------------------------------------------------------------------ */
  static uint8_t available() {
    return receive_buffer.used();
  }


private:
  static spi_mode_t mode;
  static spi_bit_order_t bit_order;

  static Queue<uint8_t, uint8_t, SPI_BUFFER_SIZE> receive_buffer;
#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
  static Queue<uint8_t, uint8_t, SPI_BUFFER_SIZE> send_buffer;
#endif
};


/* -------------------------------------------------------------------------- */
template<typename _sck, typename _miso, typename _mosi, typename _ss,
         bool slave, typename control_reg, typename status_reg, typename data_reg>
spi_mode_t HWSPI<_sck, _miso, _mosi, _ss, slave,
		 control_reg, status_reg, data_reg>::mode;

template<typename _sck, typename _miso, typename _mosi, typename _ss,
         bool slave, typename control_reg, typename status_reg, typename data_reg>
spi_bit_order_t HWSPI<_sck, _miso, _mosi, _ss, slave,
		 control_reg, status_reg, data_reg>::bit_order;

template<typename _sck, typename _miso, typename _mosi, typename _ss,
         bool slave, typename control_reg, typename status_reg, typename data_reg>
Queue<uint8_t, uint8_t, SPI_BUFFER_SIZE> HWSPI<_sck, _miso, _mosi, _ss, slave,
					       control_reg, status_reg, data_reg>::receive_buffer;

#if !defined(SPI_SLAVE_NOT_IMPLEMENTED)
template<typename _sck, typename _miso, typename _mosi, typename _ss,
         bool slave, typename control_reg, typename status_reg, typename data_reg>
Queue<uint8_t, uint8_t, SPI_BUFFER_SIZE> HWSPI<_sck, _miso, _mosi, _ss, slave,
					       control_reg, status_reg, data_reg>::send_buffer;
#endif

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#if !defined (NO_DEFAULT_SPI)
template<typename _ss = ss>
using SPI = HWSPI<sck, mosi, mosi, _ss>;

typedef SPI<ss> SPI0;
#endif
/* -------------------------------------------------------------------------- */



#endif /* HWSPI_HH */
