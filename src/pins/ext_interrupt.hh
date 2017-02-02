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

#ifndef EXT_INTERRUPT_HH
#define EXT_INTERRUPT_HH

/* -------------------------------------------------------------------------- */
/* External interrupt                                                         */
/* -------------------------------------------------------------------------- */
enum external_trigger_t {
  _external_on_low     = 0x00,
  _external_on_change  = 0x01,
  _external_on_falling = 0x02,
  _external_on_raising = 0x03
};

template<typename msk_bit = int0, typename control_bits = isc0x>
struct ExtInterrupt {
  static inline void IntOn(external_trigger_t mode) {
    control_bits::set(mode);
    msk_bit::set();
  }

  static inline void IntOff() {
    msk_bit::clear();
  }

  enum { type_id = _type_external_interrupt };
};

#endif /* EXT_INTERRUPT_HH */
