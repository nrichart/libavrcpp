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

template<uint8_t int_num>
struct ExtInterrupt {
  static inline void IntOn(external_trigger_t mode) {
    if(int_num < 6)
      eicra::sbits<2*int_num, 0x3>(mode);
#if defined(EICRB)
    else if(int_num == 6)
      eicrb::sbits<ISC60, 0x03>(mode);
#endif
    eimsk::sbit(int_num);							\
  }

  static inline void IntOff() {
    eimsk::cbit(int_num);
  }

  enum { type_id = _type_external_interrupt };
};

#endif /* EXT_INTERRUPT_HH */
