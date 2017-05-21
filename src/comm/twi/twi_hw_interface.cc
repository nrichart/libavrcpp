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
#include "twi.hh"
/* -------------------------------------------------------------------------- */
#include <avr/interrupt.h>
/* -------------------------------------------------------------------------- */

typedef twi::TWIInterface<sda, scl, twi::_hw_twi> TWIm;
/* -------------------------------------------------------------------------- */
template <typename _sda, typename _scl>
void twi::TWIInterface<_sda, _scl, twi::_hw_twi>::ext_activate() {}

template void twi::TWIInterface<sda, scl, twi::_hw_twi>::ext_activate();
/* -------------------------------------------------------------------------- */

ISR(TWI_vect) {
  TWIm::TWI_vect();
  reti();
}
