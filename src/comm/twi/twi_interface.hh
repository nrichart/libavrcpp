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
typedef twcr::bit<0> twie;
typedef twcr::bit<2> twen;
typedef twcr::bit<3> twwc;
typedef twcr::bit<4> twsto;
typedef twcr::bit<5> twsta;
typedef twcr::bit<6> twea;
typedef twcr::bit<7> twint;
/* -------------------------------------------------------------------------- */
typedef twsr::bit<0> twps0;
typedef twsr::bit<1> twps1;
typedef twsr::bits<2, 0x3> twps;
typedef twsr::bit<3> tws3;
typedef twsr::bit<4> tws4;
typedef twsr::bit<5> tws5;
typedef twsr::bit<6> tws6;
typedef twsr::bit<7> tws7;
typedef twsr::bits<2, 0x1F> tws;
/* -------------------------------------------------------------------------- */
typedef twar::bit<0> twgce;
typedef twar::bit<1> twa0;
typedef twar::bit<2> twa1;
typedef twar::bit<3> twa2;
typedef twar::bit<4> twa3;
typedef twar::bit<5> twa4;
typedef twar::bit<6> twa5;
typedef twar::bit<7> twa6;
typedef twar::bits<1, 0x7F> twa;
/* -------------------------------------------------------------------------- */
namespace clock {
enum _prescaler_t : uint8_t {
  _clk_1 = 0x00,
  _clk_4 = 0x01,
  _clk_16 = 0x02,
  _clk_64 = 0x03
};
} // clock

namespace status {
enum _status_t : uint8_t {
  _start = 0x08,
  _rep_start = 0x10,
  _mt_sla_ack = 0x18,
  _mt_sla_nack = 0x20,
  _mt_data_ack = 0x28,
  _mt_data_nack = 0x30,
  _mt_arb_lost = 0x38,
  _mr_arb_lost = 0x38,
  _mr_sla_ack = 0x40,
  _mr_sla_nack = 0x48,
  _mr_data_ack = 0x50,
  _mr_data_nack = 0x58,
  _sr_sla_ack = 0x60,
  _sr_arb_lost_sla_ack = 0x68,
  _sr_gcall_ack = 0x70,
  _sr_arb_lost_gcall_ack = 0x78,
  _sr_data_ack = 0x80,
  _sr_data_nack = 0x88,
  _sr_gcall_data_ack = 0x90,
  _sr_gcall_data_nack = 0x98,
  _sr_stop = 0xA0,
  _st_sla_ack = 0xA8,
  _st_arb_lost_sla_ack = 0xB0,
  _st_data_ack = 0xB8,
  _st_data_nack = 0xC0,
  _st_last_data = 0xC8,
  _no_info = 0xF8,
  _bus_error = 0x00
};
} // status

namespace mask {
enum : uint8_t {
  _activate = _or<twint, twsta, twsto, twea, twen, twie>::value,
  _deactivate = _or<twea, twie>::value,

  _comm = _or<twint, twsta, twsto>::value,
  _comm_ack = _or<twint, twsta, twsto, twea>::value,
  _comm_ack_slave = _or<twint, twsto, twea>::value
};
} // mask

namespace state {
enum state_t {
  _ready = 0x00,
  _write = 0x01,
  _read = 0x02,
  _gcall = 0x04,
  _master = 0x10,
  _slave = 0x20,
  _send_stop = 0x40,
  _repeat_start = 0x80
};
} // state

namespace sla {
enum : uint8_t { _write = 0, _read = 1 };
}

namespace action {
enum : uint8_t {
  _start = _or<twint, twsta>::value,
  _clear_int = twint::value,
  _stop = _or<twint, twsto>::value,
  _activate = _or<twea, twen, twie>::value,
  _nack = twint::value,
  _ack = _or<twint, twea>::value,
};
} // twcr

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
