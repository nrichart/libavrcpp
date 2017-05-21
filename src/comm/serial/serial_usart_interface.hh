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
#ifndef SERIAL_USART_INTERFACE_HH
#define SERIAL_USART_INTERFACE_HH

/* -------------------------------------------------------------------------- */
#include "common/communication.hh"
#include "common/queue.hh"
/* -------------------------------------------------------------------------- */

namespace serial {

#if (RAMEND < 1000)
#  define UART_RX_BUFFER_SIZE 16
#else
#  define UART_RX_BUFFER_SIZE 0
#endif

#define UART_TX_BUFFER_SIZE UART_RX_BUFFER_SIZE

// UCSRnA
#define RXCn 7
#define TXCn 6
#define UDREn 5

#define UPEn 2
#define U2Xn 1

// UCSRnB
#define RXCIEn 7
#define TXCIEn 6
#define UDRIEn 5
#define RXENn 4
#define TXENn 3

template <typename ucsrna, typename ucsrnb, typename ucsrnc, typename ubrrnh,
          typename ubrrnl, typename udr>
class SerialUSARTInterface
  : public Communication<
  SerialUSARTInterface<ucsrna, ucsrnb, ucsrnc, ubrrnh, ubrrnl, udr>> {
private:
  typedef SerialUSARTInterface<ucsrna, ucsrnb, ucsrnc, ubrrnh, ubrrnl, udr> my_type;

  static uint16_t computeUBRR(uint32_t baudrate, bool use_u2x) {
    if (use_u2x) {
      return ((F_CPU / 4 / baudrate - 1) / 2);
    } else {
      return ((F_CPU / 8 / baudrate - 1) / 2);
    }
  }

  static void ext_activate();
public:
  /* ------------------------------------------------------------------------ */
  static void activate(uint32_t baudrate, uint8_t baudrate_tolreance = 2) {
    uint16_t ubrr;
    bool use_u2x = true;

#if F_CPU == 16000000UL
    /* hardcoded exception for compatibility with the bootloader shipped
       with the Duemilanove and previous boards and the firmware on the 8U2
       on the Uno and Mega 2560. */
    if (baudrate == 57600) {
      use_u2x = false;
    }
#endif

    ucsrna::set(0);
    ubrr = my_type::computeUBRR(baudrate, use_u2x);

    if (ubrr > 4095 && use_u2x) {
      use_u2x = false;
      ubrr = my_type::computeUBRR(baudrate, use_u2x);
    }

    // assign the baud_setting, a.k.a. ubrr (USART Baud Rate Register)
    if (use_u2x)
      ucsrna::sbit(U2Xn);
    ubrrnh::set(ubrr >> 8);
    ubrrnl::set(ubrr & 0xFF);

    // activate receiver
    ucsrnb::sbit(TXENn);
    // activate emitter
    ucsrnb::sbit(RXENn);

// activate interruption on receive
#if UART_RX_BUFFER_SIZE > 0
    ucsrnb::sbit(RXCIEn);
#endif

#if UART_TX_BUFFER_SIZE > 0
    ucsrnb::cbit(UDRIEn);
#endif

    ext_activate();
  }

  /* ------------------------------------------------------------------------ */
  static void unactivate() {
    ucsrnb::cbit(TXENn);
    ucsrnb::cbit(RXENn);

#if UART_RX_BUFFER_SIZE > 0
    ucsrnb::cbit(RXCIEn);
#endif

#if UART_TX_BUFFER_SIZE > 0
    ucsrnb::cbit(UDRIEn);
#endif
  }

  /* ------------------------------------------------------------------------ */
  static uint8_t receiveByte() {
#if UART_RX_BUFFER_SIZE > 0
    if (receive_buffer.isEmpty())
      return 0;
    else
      return receive_buffer.pop();
#else
    uint8_t b = udr::val();
    if (ucsrna::is_clear(UPEn)) {
      return b;
    }
#endif
  }

  /* ------------------------------------------------------------------------ */
  static void sendByte(uint8_t b) {
#if UART_TX_BUFFER_SIZE > 0
    do {
    } while (send_buffer.isFull());

    send_buffer.push(b);

    ucsrnb::sbit(UDRIEn);
    ucsrna::sbit(TXCn);
#else
    ucsrna::wait_bit_is_set(UDREn);
    udr::set(b);
#endif
  }

  /* ------------------------------------------------------------------------ */
  static inline uint8_t available() {
#if UART_RX_BUFFER_SIZE > 0
    return receive_buffer.used();
#else
    return ucsrna::is_set(RXCn);
#endif
  }

/* ------------------------------------------------------------------------ */
#if UART_RX_BUFFER_SIZE > 0
  static inline void storeReceive() {
    uint8_t b = udr::val();
    if (ucsrna::is_clear(UPEn)) {
      // if no parity error
      receive_buffer.push(b);
    }
  }
#endif

/* ------------------------------------------------------------------------ */
#if UART_TX_BUFFER_SIZE > 0
  static inline void transmitSend() {
    if (send_buffer.isEmpty()) {
      ucsrnb::cbit(UDRIEn);
    } else {
      uint8_t b = send_buffer.pop();
      udr::set(b);
    }
  }
#endif

private:
#if UART_RX_BUFFER_SIZE > 0
  static Queue<uint8_t, uint8_t, UART_RX_BUFFER_SIZE> receive_buffer;
#endif
#if UART_TX_BUFFER_SIZE > 0
  static Queue<uint8_t, uint8_t, UART_TX_BUFFER_SIZE> send_buffer;
#endif
};

#if UART_RX_BUFFER_SIZE > 0
template <typename ucsrna, typename ucsrnb, typename ucsrnc, typename ubrrnh,
          typename ubrrnl, typename udr>
Queue<uint8_t, uint8_t, UART_RX_BUFFER_SIZE>
SerialUSARTInterface<ucsrna, ucsrnb, ucsrnc, ubrrnh, ubrrnl, udr>::receive_buffer;
#endif

#if UART_TX_BUFFER_SIZE > 0
template <typename ucsrna, typename ucsrnb, typename ucsrnc, typename ubrrnh,
          typename ubrrnl, typename udr>
Queue<uint8_t, uint8_t, UART_TX_BUFFER_SIZE>
SerialUSARTInterface<ucsrna, ucsrnb, ucsrnc, ubrrnh, ubrrnl, udr>::send_buffer;
#endif

}  // serial

#endif /* SERIAL_USART_INTERFACE_HH */
