/*
 * Adaptation of the NewSoftSerial : Multi-instance software serial library
 * Copyright (c) 2006 David A. Mellis.  All rights reserved.
 * -- Interrupt-driven receive and other improvements by ladyada
 * -- Tuning, circular buffer, derivation from class Print,
 *    multi-instance support, porting to 8MHz processors,
 *    various optimizations, PROGMEM delay tables, inverse logic and
 *    direct port writing by Mikal Hart
 * -- Integration in libavrc++ by netWorms
 */

/* -------------------------------------------------------------------------- */
#ifndef SERIAL_SOFTWARE_INTERFACE_HH
#define SERIAL_SOFTWARE_INTERFACE_HH

/* -------------------------------------------------------------------------- */
#include "common/common.hh"
#include "common/queue.hh"
/* -------------------------------------------------------------------------- */
#include <avr/pgmspace.h>
/* -------------------------------------------------------------------------- */

namespace serial {

/* -------------------------------------------------------------------------- */
typedef struct _DELAY_TABLE {
  long baud;
  unsigned short rx_delay_centering;
  unsigned short rx_delay_intrabit;
  unsigned short rx_delay_stopbit;
  unsigned short tx_delay;
} DELAY_TABLE;

extern const DELAY_TABLE PROGMEM sw_serial_delay_table[];
extern const uint8_t sizeof_sw_serial_delay_table;
extern const int XMIT_START_ADJUSTMENT;
/* -------------------------------------------------------------------------- */
#if (RAMEND < 1000)
#define RX_BUFFER_SIZE 16
#else
#define RX_BUFFER_SIZE 64
#endif

/* -------------------------------------------------------------------------- */
template <typename tx, typename rx, uint8_t size = RX_BUFFER_SIZE>
class SerialSoftwareInterface {
public:
  static void activate(unsigned long baudrate, bool inverse_logic = false);

  void activate();

  /* ------------------------------------------------------------------------ */
  static inline uint8_t receiveByte() {
    // if(!external_on) storeReceive();
    return receive_buffer.pop();
  };

  /* ------------------------------------------------------------------------ */
  static inline void sendByte(uint8_t b);

  /* ------------------------------------------------------------------------ */
  static inline uint8_t available() { return receive_buffer.used(); }

  /* ------------------------------------------------------------------------ */
  static inline void storeReceive();

private:
  static inline void tunedDelay(uint16_t delay) {
    uint8_t tmp = 0;
    asm volatile("sbiw    %0, 0x01 \n\t"
                 "ldi %1, 0xFF \n\t"
                 "cpi %A0, 0xFF \n\t"
                 "cpc %B0, %1 \n\t"
                 "brne .-10 \n\t"
                 : "+r"(delay), "+a"(tmp)
                 : "0"(delay));
  }

private:
  static unsigned short rx_delay_centering;
  static unsigned short rx_delay_intrabit;
  static unsigned short rx_delay_stopbit;
  static unsigned short tx_delay;

  static bool inverse_logic;
  static bool buffer_overflow;
  static bool external_on;

  static Queue<uint8_t, uint8_t, size> receive_buffer;
};

/* -------------------------------------------------------------------------- */
/* Template Code                                                              */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
// template<typename tx, typename rx, uint8_t size>
// void SerialSoftwareInterface<tx, rx, size>::activate() {
//   uint8_t oldSREG = SREG;
//   cli();
//   active = this;
//   SREG = oldSREG;
// }

/* -------------------------------------------------------------------------- */
template <typename tx, typename rx, uint8_t size>
void SerialSoftwareInterface<tx, rx, size>::activate(unsigned long baudrate,
                                                     bool inv_logic) {
  inverse_logic = inv_logic;
  buffer_overflow = false;

  tx::output();
  tx::high();

  rx::input();
  if (!inverse_logic)
    rx::high(); // pullup for normal logic!

  rx_delay_centering = rx_delay_intrabit = rx_delay_stopbit = tx_delay = 0;

  for (unsigned i = 0; i < sizeof_sw_serial_delay_table; ++i) {
    unsigned long baud = pgm_read_dword(&sw_serial_delay_table[i].baud);
    if (baud == baudrate) {
      rx_delay_centering =
          pgm_read_word(&sw_serial_delay_table[i].rx_delay_centering);
      rx_delay_intrabit =
          pgm_read_word(&sw_serial_delay_table[i].rx_delay_intrabit);
      rx_delay_stopbit =
          pgm_read_word(&sw_serial_delay_table[i].rx_delay_stopbit);
      tx_delay = pgm_read_word(&sw_serial_delay_table[i].tx_delay);
      break;
    }
  }

  external_on = false;
  // if(rx::hasExternal()) {
  //   external_on = true;
  //   if(inverse_logic)
  //     rx::externalIntOn(_external_on_raising);
  //   else
  //     rx::externalIntOn(_external_on_falling);
  // }

  // Set up RX interrupts, but only if we have a valid RX baud rate
  if (rx_delay_stopbit) {
    // rx::PCIntOn();
    tunedDelay(tx_delay); // if we were low this establishes the end
  }
}

/* -------------------------------------------------------------------------- */
//
// The receive routine called by the interrupt handler
//
template <typename tx, typename rx, uint8_t size>
void SerialSoftwareInterface<tx, rx, size>::storeReceive() {
  uint8_t d = 0;

  // If RX line is high, then we don't see any start bit
  // so interrupt is probably not for us
  if (inverse_logic ? rx::read() : !rx::read()) {
    // Wait approximately 1/2 of a bit width to "center" the sample
    tunedDelay(rx_delay_centering);

    // Read each of the 8 bits
    for (uint8_t i = 0x1; i; i <<= 1) {
      tunedDelay(rx_delay_intrabit);
      uint8_t noti = ~i;
      if (rx::read())
        d |= i;
      else // else clause added to ensure function timing is ~balanced
        d &= noti;
    }

    // skip the stop bit
    tunedDelay(rx_delay_stopbit);

    if (inverse_logic)
      d = ~d;

    // if buffer full, set the overflow flag and return
    if (!receive_buffer.isFull()) {
      // save new data in buffer: tail points to where byte goes
      receive_buffer.push(d); // save new byte
    } else {
      buffer_overflow = true;
    }
  }
}

/* -------------------------------------------------------------------------- */
template <typename tx, typename rx, uint8_t size>
void SerialSoftwareInterface<tx, rx, size>::sendByte(uint8_t b) {
  if (tx_delay == 0)
    return;

  uint8_t oldSREG = SREG;
  cli(); // turn off interrupts for a clean txmit

  // Write the start bit
  tx::write(inverse_logic ? HIGH : LOW);

  tunedDelay(tx_delay + XMIT_START_ADJUSTMENT);

  // Write each of the 8 bits
  if (inverse_logic) {
    for (uint8_t mask = 0x01; mask; mask <<= 1) {
      if (b & mask) // choose bit
        tx::low();  // send 1
      else
        tx::high(); // send 0

      tunedDelay(tx_delay);
    }
    tx::low(); // restore pin to natural state
  } else {
    for (uint8_t mask = 0x01; mask; mask <<= 1) {
      if (b & mask) // choose bit
        tx::high(); // send 1
      else
        tx::low(); // send 0

      tunedDelay(tx_delay);
    }
    tx::high(); // restore pin to natural state
  }
  SREG = oldSREG; // turn interrupts back on
  tunedDelay(tx_delay);
}

/* -------------------------------------------------------------------------- */
template <typename tx, typename rx, uint8_t size>
bool SerialSoftwareInterface<tx, rx, size>::inverse_logic = false;

template <typename tx, typename rx, uint8_t size>
bool SerialSoftwareInterface<tx, rx, size>::buffer_overflow = false;

template <typename tx, typename rx, uint8_t size>
bool SerialSoftwareInterface<tx, rx, size>::external_on = false;

template <typename tx, typename rx, uint8_t size>
unsigned short SerialSoftwareInterface<tx, rx, size>::rx_delay_centering = 0;

template <typename tx, typename rx, uint8_t size>
unsigned short SerialSoftwareInterface<tx, rx, size>::rx_delay_intrabit = 0;

template <typename tx, typename rx, uint8_t size>
unsigned short SerialSoftwareInterface<tx, rx, size>::rx_delay_stopbit = 0;

template <typename tx, typename rx, uint8_t size>
unsigned short SerialSoftwareInterface<tx, rx, size>::tx_delay = 0;

template <typename tx, typename rx, uint8_t size>
Queue<uint8_t, uint8_t, size> SerialSoftwareInterface<tx, rx, size>::receive_buffer;

/* -------------------------------------------------------------------------- */
#define DeclareInteruptReceive(tx, rx) DeclareInteruptRecieve_##rx(tx)

#define DeclareInteruptReceive_pin2(tx)                                        \
  ISR(INT0_vect) { SerialSoftwareInterface<tx, pin2>::storeReceive(); }

#define DeclareInteruptReceive_pin3(tx)                                        \
  ISR(INT0_vect) { SwSerial<tx, pin3>::storeReceive(); }

} // serial

#endif /* SERIAL_SOFTWARE_INTERFACE_HH */
