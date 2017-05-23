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
#ifndef TWI_INTERFACE_HH
#define TWI_INTERFACE_HH

/* -------------------------------------------------------------------------- */
#include "common/common.hh"
#include "twi/twi.hh"
/* -------------------------------------------------------------------------- */

namespace twi {

/* -------------------------------------------------------------------------- */
template <typename _sda, typename _scl, twi_interface_t interface>
class TWIInterface { };

} // namespace twi

/* -------------------------------------------------------------------------- */
#if defined(USI)
#include "twi/twi_usi_interface.hh"
#endif

#if defined(TWAR)
#include "twi/twi_hw_interface.hh"
#endif
/* -------------------------------------------------------------------------- */

#endif /* TWI_INTERFACE_HH */
