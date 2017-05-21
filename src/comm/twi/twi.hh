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

#ifndef __TWI_HH__
#define __TWI_HH__

/* -------------------------------------------------------------------------- */
#include <util/twi.h>
#include <avr/interrupt.h>
/* -------------------------------------------------------------------------- */

#if !defined(TWI_RECEIVE_BUFFER_SIZE)
#define TWI_RECEIVE_BUFFER_SIZE 32
#elif TWI_RECEIVE_BUFFER_SIZE > 255
#define TWI_RECEIVE_BUFFER_SIZE 255
#endif

#if !defined(TWI_SEND_BUFFER_SIZE)
#define TWI_SEND_BUFFER_SIZE 32
#elif TWI_SEND_BUFFER_SIZE > 255
#define TWI_SEND_BUFFER_SIZE 255
#endif

/* -------------------------------------------------------------------------- */
namespace twi {
enum twi_interface_t {
  _usi_twi,
  _hw_twi
};
}

#if !defined(TWI_INTERFACE)
#  define TWI_INTERFACE twi::_hw_twi
#endif

/* -------------------------------------------------------------------------- */
#include "twi/twi_interface.hh"
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* TWI                                                                        */
/* -------------------------------------------------------------------------- */
template <uint8_t addr, typename _sda, typename _scl, uint8_t slave_addr = 0x00,
          twi::twi_interface_t _interface = twi::_hw_twi>
class TWI : public twi::TWIInterface<_sda, _scl, _interface> {
public:
  typedef twi::TWIInterface<_sda, _scl, _interface> interface;

  /* ------------------------------------------------------------------------ */
  static void
  activate(bool slave = false, uint32_t freq = 100000L,
           twi::clock::_prescaler_t prescaler = twi::clock::_clk_1) {
    // set the bite rate
    interface::setPrescaler(prescaler, freq);

    interface::activate(addr, slave_addr, slave);
  }

  static void deactivate() {
    _sda::low();
    _scl::low();
    interface::deactivate();
  }

  /* ------------------------------------------------------------------------ */
  template <typename T> static void registerRead(uint8_t reg, T &value) {
    send(reg, twi::_non_stop);
    receive(value, twi::_stop);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static void registerRead(uint8_t reg, T value[], uint8_t size) {
    send(reg, twi::_non_stop);
    receive(value, size, twi::_stop);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T> static void registerWrite(uint8_t reg, const T &value) {
    interface::send(reg);
    interface::send(value);
    sendTo();
  }

  /* ------------------------------------------------------------------------ */
  static inline void sendTo(twi::_stop_t s = twi::_stop) {
    interface::_sendTo(addr, s);
  }

  /* ------------------------------------------------------------------------ */
  static inline void receiveFrom(uint8_t size, twi::_stop_t s = twi::_stop) {
    interface::_receiveFrom(addr, size, s);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static inline void send(const T &data, twi::_stop_t s = twi::_stop) {
    // pack the data
    interface::send(data);
    // send them
    sendTo(s);
  }

  template <typename T>
  static inline void send(const T data[], uint8_t size,
                          twi::_stop_t s = twi::_stop) {
    // pack the data
    interface::send(data, size);
    // send them
    sendTo(s);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static inline void receive(T &data, twi::_stop_t s = twi::_stop) {
    // get the data
    receiveFrom(sizeof(T), s);
    // unpack them
    interface::receive(data);
  }

  template <typename T>
  static inline void receive(T data[], uint8_t size,
                             twi::_stop_t s = twi::_stop) {
    // get the data
    receiveFrom(size * sizeof(T), s);
    // unpack them
    interface::receive(data, size);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static inline T transfer(const T &sdata, twi::_stop_t s = twi::_stop) {
    send(sdata, twi::_non_stop);
    T rdata;
    receive(rdata, s);
    return rdata;
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static inline void transfer(const T sdata[], T rdata[], uint8_t ssize,
                              uint8_t rsize, twi::_stop_t s = twi::_stop) {
    send(sdata, ssize, twi::_non_stop);
    receive(rdata, rsize, s);
  }
};

/* -------------------------------------------------------------------------- */
#if !defined(NO_DEFAULT_TWI)
template<uint8_t addr, uint8_t slave_addr = 0x00>
using TWI0 = TWI<addr, sda, scl, slave_addr, TWI_INTERFACE>;
#endif
/* -------------------------------------------------------------------------- */

#endif //__TWI_HH__
