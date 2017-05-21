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

#include "common/common.hh"

#if defined(TIMER0)
ISR(TIMER0_OVF_vect) {
  Timer0::onOVF();
}
#endif

#if defined(TIMER1)
ISR(TIMER1_OVF_vect) {
  Timer1::onOVF();
}
#endif

#if defined(TIMER2)
ISR(TIMER2_OVF_vect) {
  Timer2::onOVF();
}
#endif

#if defined(TIMER3)
ISR(TIMER3_OVF_vect) {
  Timer3::onOVF();
}
#endif

#if defined(TIMER4)
ISR(TIMER4_OVF_vect) {
  Timer4::onOVF();
}
#endif
