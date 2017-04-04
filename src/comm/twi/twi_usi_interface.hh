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
#include "twi/twi_interface.hh"
/* -------------------------------------------------------------------------- */

#ifndef TWI_USI_INTERFACE_HH
#define TWI_USI_INTERFACE_HH

namespace twi {

template <typename _sda, typename _scl>
class TWIInterface<_sda, _scl, _usi_twi>
    : public Communication<TWIInterface<_sda, _scl, _usi_twi>> {

  typedef Communication<TWIInterface<_sda, _scl, _usi_twi>> CommLayer;

public:
  static void activate(uint8_t addr, uint8_t slave_addr, bool _slave = false) {}
};

/* -------------------------------------------------------------------------- */


} // namespace twi

#endif /* TWI_USI_INTERFACE_HH */
