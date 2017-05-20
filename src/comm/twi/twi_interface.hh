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
/* -------------------------------------------------------------------------- */

#ifndef TWI_INTERFACE_HH
#define TWI_INTERFACE_HH

namespace twi {

enum _stop_t { _non_stop = 0x00, _stop = 0x01 };

/* -------------------------------------------------------------------------- */
namespace clock {
enum _prescaler_t : uint8_t {
  _clk_1 = 0x00,
  _clk_4 = 0x01,
  _clk_16 = 0x02,
  _clk_64 = 0x03
};
} // clock

namespace sla {
enum : uint8_t { _write = 0, _read = 1 };
}

/* -------------------------------------------------------------------------- */
template <typename _sda, typename _scl, twi_interface_t interface>
class TWIInterface : public Communication<TWIInterface<_sda, _scl, interface>> {
};

/* -------------------------------------------------------------------------- */

} // namespace twi

/* -------------------------------------------------------------------------- */
#include "twi/twi_hw_interface.hh"
#include "twi/twi_usi_interface.hh"
/* -------------------------------------------------------------------------- */

#endif /* TWI_INTERFACE_HH */
