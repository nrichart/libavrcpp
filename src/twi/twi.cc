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

typedef TWIInterface<sda, scl> TWIm;

using namespace twi;
using namespace twi::status;

ISR(TWI_vect) {
  //  uint8_t sreg = SREG;
  //  cli();

  switch (TWIm::status()) {
  case _start:
  case _rep_start: {
    if (TWIm::state & twi::state::_write)
      TWIm::sla(sla::_write);
    else
      TWIm::sla(sla::_read);

    TWIm::clearInt();
    break;
  }
  /* -- Master transmit ------------------------------------------------- */
  case _mt_sla_ack:   // slave acked address
  case _mt_data_ack: {// slave acked data
    // if still data to send, send the next byte
    if (TWIm::nb_send) {
      TWIm::send_stored();
      TWIm::clearInt();
    } else {
      // else stop or repeat start
      if (TWIm::state & twi::state::_send_stop)
        TWIm::stop(_stop);
      else
        TWIm::repeatStart();
    }
    break;
  }
  case _mt_sla_nack:
  case _mt_data_nack:
    // should probably handle the error;
    TWIm::stop(twi::_stop);
    break;
  case _mt_arb_lost:
    // lost arbitration, send a start to try again once the bus is free
    TWIm::start();
    break;
  case _mr_data_ack:
    TWIm::store_received();
  case _mr_sla_ack:
    if (TWIm::nb_to_recieve == 0) {
      TWIm::nack();
    } else {
      TWIm::ack();
    }
    break;
  // -- Master receive -------------------------------------------------- */
  case _mr_sla_nack:
    // should probably handle the error;
    TWIm::stop(twi::_stop);
    break;
  case _mr_data_nack:
    TWIm::store_received();
    if (TWIm::state & twi::state::_send_stop)
      TWIm::stop();
    else
      TWIm::repeatStart();
    break;
  case _sr_gcall_ack:
  case _sr_arb_lost_gcall_ack:
    TWIm::state |= twi::state::_gcall;
  case _sr_sla_ack:
  case _sr_arb_lost_sla_ack:
    TWIm::state |= twi::state::_slave | twi::state::_read;
    TWIm::ack();
    break;
  case _sr_data_ack:
  case _sr_gcall_data_ack:
    if(TWIm::receive_buffer.isFull()) {
      TWIm::nack(false);
    } else {
      TWIm::store_received();
      TWIm::ack(false);
    }
    break;
  case _sr_data_nack:
  case _sr_gcall_data_nack:
    TWIm::store_received();
    TWIm::nack();
    break;
  case _sr_stop:
    TWIm::ack();

    TWIm::callbackRead();
    TWIm::state = twi::state::_ready;
    break;
  case _st_sla_ack:
  case _st_arb_lost_sla_ack:
    TWIm::state |= twi::state::_slave | twi::state::_write;
    TWIm::callbackWrite();
  case _st_data_ack:
    TWIm::send_stored();
    if (TWIm::nb_send) TWIm::ack(false);
    else TWIm::nack(false);
    break;
  case _st_data_nack:
  case _st_last_data:
    TWIm::ack();
    TWIm::state = twi::state::_ready;
    break;
  case _no_info:
    break;
  case _bus_error:
    TWIm::stop();
    break;
  }
  //  SREG = sreg;
}
