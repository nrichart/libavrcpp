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

#include <avr/interrupt.h>
#include "common/common.hh"
#include "hw_serial.hh"

#if UART_RX_BUFFER_SIZE > 0
#if defined(USART_RX_vect)
SIGNAL(USART_RX_vect)
{
  Serial0::storeReceive();
}
#endif

#if defined(USART1_RX_vect)
SIGNAL(USART1_RX_vect)
{
  Serial1::storeReceive();
}
#endif
#endif

#if UART_TX_BUFFER_SIZE > 0
#if defined(USART_UDRE_vect)
ISR(USART_UDRE_vect)
{
  Serial0::transmitSend();
}
#endif

#if defined(USART1_UDRE_vect)
ISR(USART1_UDRE_vect)
{
  Serial1::transmitSend();
}
#endif
#endif
