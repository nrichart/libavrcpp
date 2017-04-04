#include "common/common.hh"
#include "serial/sw_serial.hh"

/*
 * Adaptation of the NewSoftSerial : Multi-instance software serial library
 * Copyright (c) 2006 David A. Mellis.  All rights reserved.
 * -- Interrupt-driven receive and other improvements by ladyada
 * -- Tuning, circular buffer, derivation from class Print,
 *    multi-instance support, porting to 8MHz processors,
 *    various optimizations, PROGMEM delay tables, inverse logic and
 *    direct port writing by Mikal Hart
 * -- Tuning of the delay for the libavrc++ by netWorms
 */

//
// Lookup table
//

#if F_CPU == 16000000

const DELAY_TABLE PROGMEM sw_serial_delay_table[] = {
  //  baud    rxcenter   rxintra    rxstop    tx
  { 115200,   1,         17,        17,       12,    },
  { 57600,    10,        37,        37,       33,    },
  { 19200,    54,        117,       117,      114,   },
  { 9600,     114,       236,       236,      233,   },
  { 4800,     233,       474,       474,      471,   },
  { 2400,     471,       950,       950,      947,   },
};
const int XMIT_START_ADJUSTMENT = 5;

#elif F_CPU == 8000000
const DELAY_TABLE PROGMEM sw_serial_delay_table[] = {
  //  baud    rxcenter    rxintra    rxstop  tx
  { 115200,   1,          5,         5,      3,      },
  { 57600,    1,          15,        15,     13,     },
  { 19200,    20,         55,        55,     52,     },
  { 9600,     50,         114,       114,    112,    },
  { 4800,     110,        233,       233,    230,    },
  { 2400,     229,        472,       472,    469,    },
};

const int XMIT_START_ADJUSTMENT = 4;
#else
#error This version of SwSerial supports only 8MHz processors
#endif

const uint8_t sizeof_sw_serial_delay_table = 6;

/* -------------------------------------------------------------------------- */
/* static */
/* -------------------------------------------------------------------------- */
