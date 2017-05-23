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

#ifndef TWI_USI_INTERFACE_HH
#define TWI_USI_INTERFACE_HH

/* -------------------------------------------------------------------------- */
#include "common/common.hh"
#include "common/communication.hh"
#include "common/queue.hh"
#include "twi/twi_interface.hh"
/* -------------------------------------------------------------------------- */

namespace twi {

template <typename _sda, typename _scl>
class TWIInterface<_sda, _scl, _usi_twi>
  : public Communication<TWIInterface<_sda, _scl, _usi_twi>> {
private:
  typedef TWIInterface<_sda, _scl, _usi_twi> my_type;
  typedef Communication<my_type> CommLayer;

public:
  static void activate(uint8_t addr, uint8_t slave_addr, bool _slave = false) {}
  static void deactivate(uint8_t addr, bool _slave = false) {}

  static inline void setPrescaler(clock::_prescaler_t prescaler,
                                  uint32_t freq) {}

  static inline void sendByte(uint8_t) {}
  static inline uint8_t receiveByte() { return 0; }
  static inline uint8_t transferByte(uint8_t) { return 0; }

  template <typename T> static inline void send(const T &) {}
  template <typename T> static inline void send(const T[], uint8_t) {}
  template <typename T> static inline void receive(T &) {}
  template <typename T> static inline void receive(T[], uint8_t) {}
  static inline void sendTo(uint8_t, stop_t = _stop) {}
  static inline void receiveFrom(uint8_t, uint8_t, stop_t = _stop) {}
};

/* -------------------------------------------------------------------------- */

} // namespace twi

#endif /* TWI_USI_INTERFACE_HH */
