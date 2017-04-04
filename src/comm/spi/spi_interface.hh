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
#include "common/communication.hh"
/* -------------------------------------------------------------------------- */

#ifndef SPI_INTERFACE_HH
#define SPI_INTERFACE_HH

namespace spi {
enum spi_interface_t {
  _hw_spi,
  _sw_spi
};

enum _bit_order_t { _bo_msb = 0, _bo_lsb = 1 };

enum _clock_polarity_t { _cpol_rising = 0, _cpol_falling = 1 };

enum _clock_phase_t { _cpha_sample = 0, _cpha_setup = 1 };

enum _mode_t {
  _mode_0 = (_cpol_rising << 1) | _cpha_sample,
  _mode_1 = (_cpol_rising << 1) | _cpha_setup,
  _mode_2 = (_cpol_falling << 1) | _cpha_sample,
  _mode_3 = (_cpol_falling << 1) | _cpha_setup
};
/* -------------------------------------------------------------------------- */

template<typename _sck, typename _miso, typename _mosi, typename _ss,
	 spi_interface_t _interface = spi::_hw_spi>
class SPIInterface : public Communication<SPIInterface<_sck, _miso, _mosi, _ss, _interface>> {
};

}  // spi

/* -------------------------------------------------------------------------- */
#include "spi/spi_hw_interface.hh"
#include "spi/spi_sw_interface.hh"
/* -------------------------------------------------------------------------- */
#endif // SPI_INTERFACE_HH
