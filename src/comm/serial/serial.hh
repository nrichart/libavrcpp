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
#ifndef SERIAL_HH
#define SERIAL_HH
/* -------------------------------------------------------------------------- */

namespace serial {
enum serial_interface_t {
  _sw_serial,
  _usart_serial,
};
}

/* -------------------------------------------------------------------------- */
#if (RAMEND < 1000)
#define RX_BUFFER_SIZE 16
#else
#define RX_BUFFER_SIZE 64
#endif

/* -------------------------------------------------------------------------- */
#include "serial_usart_interface.hh"
#include "serial_software_interface.hh"
/* -------------------------------------------------------------------------- */

#if defined(SERIAL_INTERFACE_0)
using Serial0 = typename std::conditional<SERIAL_INTERFACE_0 == serial::_usart_serial,
                                          serial::SerialUSARTInterface<ucsr0a, ucsr0b, ucsr0c, ubrr0h, ubrr0l, udr0>,
                                          void>::type;
#endif

#if defined(SERIAL_INTERFACE_1)
using Serial1 = typename std::conditional<SERIAL_INTERFACE_0 == serial::_usart_serial,
                                          serial::SerialUSARTInterface<ucsr1a, ucsr1b, ucsr1c, ubrr1h, ubrr1l, udr1>,
                                          void>::type;
#endif

template <typename tx, typename rx, uint8_t size = RX_BUFFER_SIZE>
using Serial = serial::SerialSoftwareInterface<tx, rx, size>;

#endif /* SERIAL_HH */
