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
#include "common/common.hh"
#include <stdint.h>
/* -------------------------------------------------------------------------- */

#ifndef PC_INTERRUPT_HH
#define PC_INTERRUPT_HH

/* -------------------------------------------------------------------------- */
template<typename pcintport, uint8_t pin_num>
struct PinChangeInterruptPin {
  static void pinChangeInterruptOn() {
    pcintport::pinChangeInterruptOn(pin_num);
  }
  static void pinChangeInterruptOff() {
    pcintport::pinChangeInterruptOff(pin_num);
  }
};

/* -------------------------------------------------------------------------- */
template<typename pcmskx, typename pcie_bit>
class PinChangeInterruptPort {
public:
  static inline void pinChangeInterruptOn(uint8_t pin) {
    pcie_bit::set();
    pcmskx::sbit(pin);
  }

  static inline void pinChangeInterruptOff(uint8_t pin) {
    pcmskx::cbit(pin);
  }

  template <uint8_t pin>
  using pin = PinChangeInterruptPin<PinChangeInterruptPort<pcmskx, pcie_bit>, pin>;
};

/* -------------------------------------------------------------------------- */
template<uint8_t pin_num>
class PinChangeInterruptPin<unused_type, pin_num> { };

#endif /* PC_INTERRUPT_HH */
