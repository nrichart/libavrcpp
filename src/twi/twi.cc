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

#include "twi.hh"

ISR(TWI_vect)
{
  //  uint8_t sreg = SREG;
  //  cli();

  typedef TWIInterface<sda, scl, false> TWIm;
  switch(TW_STATUS)
    {
    case TWIm::_start                 :
    case TWIm::_rep_start	      :
      if(TWIm::state & TWIm::_write)
	twdr::set(TW_WRITE | (TWIm::addr << 1));
      else
	twdr::set(TW_READ  | (TWIm::addr << 1));
      TWIm::clearInt();
      break;

      /* -- Master transmit ------------------------------------------------- */
    case TWIm::_mt_sla_ack		: // slave acked address
    case TWIm::_mt_data_ack		: // slave acked data
      // if still data to send, send the next byte
      if(TWIm::nb_send) {
	uint8_t data = TWIm::send_buffer.pop();
	twdr::set(data);
	--TWIm::nb_send;
	TWIm::clearInt();
      } else {
	// else stop or repeat start
	if(TWIm::state & TWIm::_send_stop)
	  TWIm::stop();
	else TWIm::repeatStart();
      }
      break;
    case TWIm::_mt_sla_nack		:
    case TWIm::_mt_data_nack		:
      // should probably handle the error;
      TWIm::stop(twi_stop);
      break;
    case TWIm::_mt_arb_lost		:
      // lost arbitration, send a start to try again once the bus is free
      TWIm::start();
      break;

      // -- Master receive -------------------------------------------------- */
    case TWIm::_mr_sla_nack		:
      // should probably handle the error;
      TWIm::stop(twi_stop);
      break;
    case TWIm::_mr_data_nack		:
      TWIm::store_received();
      if(TWIm::state & TWIm::_send_stop)
	TWIm::stop();
      else TWIm::repeatStart();
      break;
    case TWIm::_mr_data_ack		:
      TWIm::store_received();
    case TWIm::_mr_sla_ack		:
      if(TWIm::nb_to_recieve == 0) {
	TWIm::nack();
      } else {
	TWIm::ack();
      }
      break;
      // case TWIm::_mr_arb_lost		:
      //   // lost arbitration, send a start to try again once the bus is free
      //   TWIm::start();
      //   break;

      /* -- Slave transmit -------------------------------------------------- */
    case TWIm::_st_sla_ack		:
    case TWIm::_st_arb_lost_sla_ack	:
    case TWIm::_st_data_ack		:
    case TWIm::_st_data_nack		:
    case TWIm::_st_last_data		:
    case TWIm::_sr_sla_ack		:
    case TWIm::_sr_arb_lost_sla_ack	:
    case TWIm::_sr_gcall_ack		:
    case TWIm::_sr_arb_lost_gcall_ack	:
    case TWIm::_sr_data_ack		:
    case TWIm::_sr_data_nack		:
    case TWIm::_sr_gcall_data_ack	:
    case TWIm::_sr_gcall_data_nack	:
    case TWIm::_sr_stop		        :
    case TWIm::_no_info		        :
    case TWIm::_bus_error		:
      break;
    }

  //  SREG = sreg;
}
