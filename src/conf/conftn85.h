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


#ifndef CONFTN85_HH
#define CONFTN85_HH

/*
  ATMEL ATTiny85

                                               +-\/-+
 PCINT5 / RESET  / dW          / ADC0 (D5) PB5 1|    |8  Vcc
 PCINT3 / XTA/L1 / CLKI / OC1B / ADC3 (D3) PB3 2|    |7  PB2 (D2) SCK  / USCK / SCL  / ADC1 / T0   / INT0 / PCINT2
 PCINT4 / XTAL2  / CLKO / OC1B / ADC2 (D4) PB4 3|    |6  PB1 (D1) MISO / DO   /        AIN1 / OC0B / OC1A / PCINT1
                                           GND 4|    |5  PB0 (D0) MOSI / DI   / SDA  / AIN0 / OC0A / OC1A / PCINT0 / AREF
                                                +----+

*/

/* -------------------------------------------------------------------------- */
/* Registers definitions                                                      */
/* -------------------------------------------------------------------------- */
#include "pins/registers.hh"

typedef reg_io<0x03> adcsrb;
#ifndef __ASSEMBLER__
typedef reg_io<0x04, uint16_t> adc;
#endif
typedef reg_io<0x04, uint16_t> adcw;
typedef reg_io<0x04> adcl;
typedef reg_io<0x05> adch;
typedef reg_io<0x06> adcsra;
typedef reg_io<0x07> admux;
typedef reg_io<0x08> acsr;
/* Reserved [0x09..0x0C] */
typedef reg_io<0x0D> usicr;
typedef reg_io<0x0E> usisr;
typedef reg_io<0x0F> usidr;
typedef reg_io<0x10> usibr;
typedef reg_io<0x11> gpior0;
typedef reg_io<0x12> gpior1;
typedef reg_io<0x13> gpior2;
typedef reg_io<0x14> didr0;
typedef reg_io<0x15> pcmsk;
typedef reg_io<0x16> pinb;
typedef reg_io<0x17> ddrb;
typedef reg_io<0x18> portb;
/* Reserved [0x19..0x1B] */
/* EEPROM Control Register EECR */
typedef reg_io<0x1C, uint8> eecr;
/* EEPROM Data Register */
typedef reg_io<0x1D, uint8> eedr;
/* EEPROM Address Register */
typedef reg_io<0x1E, uint16_t> eear;
typedef reg_io<0x1E> eearl;
typedef reg_io<0x1F> eearh;
typedef reg_io<0x20> prr;
typedef reg_io<0x21> wdtcr;
typedef reg_io<0x22> dwdr;
typedef reg_io<0x23> dtps1;
typedef reg_io<0x24> dt1b;
typedef reg_io<0x25> dt1a;
typedef reg_io<0x26> clkpr;
typedef reg_io<0x27> pllcsr;
typedef reg_io<0x28> ocr0b;
typedef reg_io<0x29> ocr0a;
typedef reg_io<0x2A> tccr0a;
typedef reg_io<0x2B> ocr1b;
typedef reg_io<0x2C> gtccr;
typedef reg_io<0x2D> ocr1c;
typedef reg_io<0x2E> ocr1a;
typedef reg_io<0x2F> tcnt1;
typedef reg_io<0x30> tccr1;
typedef reg_io<0x31> osccal;
typedef reg_io<0x32> tcnt0;
typedef reg_io<0x33> tccr0b;
typedef reg_io<0x34> mcusr;
typedef reg_io<0x35> mcucr;
/* Reserved [0x36] */
typedef reg_io<0x37> spmcsr;
typedef reg_io<0x38> tifr;
typedef reg_io<0x39> timsk;
typedef reg_io<0x3A> gifr;
typedef reg_io<0x3B> gimsk;

typedef bit<adcsra, ADSC> adsc;
typedef bit<adcsra, ADEN> aden;

/* -------------------------------------------------------------------------- */
/* ADC definitions                                                            */
/* -------------------------------------------------------------------------- */
#include "pins/adc.hh"

typedef ADCPort<0x00> adc0;
typedef ADCPort<0x01> adc1;
typedef ADCPort<0x02> adc2;
typedef ADCPort<0x03> adc3;
typedef ADCPort<0x0F> adc4;
typedef ADCPort<0x0C> adcvbg;
typedef ADCPort<0x0D> adcgnd;
typedef ADCPort<0x0F> adctemp;

/* -------------------------------------------------------------------------- */
/* Timers                                                                     */
/* -------------------------------------------------------------------------- */
#include "pins/timer.hh"

typedef timer_prescaler_t0_t timer0_prescaler_t;
typedef timer_prescaler_t4_t timer1_prescaler_t;

typedef TimerCounterControl<timer0_prescaler_t, timer_waveform_t0_t, tccr0a, tccr0b> tcc0;
typedef OutputCompare<tcc0, ocr0a, ocr0b> OC0;

typedef bit<timsk, TOIE0> toie0;
typedef bit<timsk, TOIE1> toie1;

typedef Timer<OC0, tcnt0, toie0, 0> Timer0;

template<timer_channel_t channel>
class TimerChannelTimyx5 {
public:
  typedef typename timer::prescaler_t prescaler_t;

  static inline void setPrescaler(prescaler_t prescaler)
  { tccr1::sbits<CS10,0xF>(prescaler); }
  static inline void setMode(timer_mode_t mode) {
    switch(channel) {
    case _timer_channel_a: tccr1::sbits<COM1A0, 0x3>(val);
    case _timer_channel_b: gtccr::sbits<COM1B0, 0x3>(val);
    }
  }

  static inline void setOC(uint8_t val) {
    switch(channel) {
    case _timer_channel_a: ocr1a::set(val);
    case _timer_channel_b: ocr1b::set(val);
    }
  }

  static inline void enableOverflowInterrupt(user_ovf_t  ovf) {
    toie1::set();
    user_ovf = ovf;
  }

  enum { type_id = _type_timer_channel };
private:
  static user_ovf_t user_ovf;
};

template<timer_channel_t channel>
user_ovf_t TimerChannelTimyx5<channel>::user_ovf = NULL;

typedef TimerChannel<Timer0, _timer_channel_a> oc0a;
typedef TimerChannel<Timer0, _timer_channel_b> oc0b;
typedef TimerChannelTimyx5<_timer_channel_a> oc1a;
typedef TimerChannelTimyx5<_timer_channel_b> oc1b;

/* -------------------------------------------------------------------------- */
/* External Interrupt                                                         */
/* -------------------------------------------------------------------------- */
#include "pins/ext_interrupt.hh"

typedef bit<gimsk, INTO> int0;
typedef bits<mcucr, ISC00, 0x3> isc0x;

typedef ExtInterrupt<int0, isc0x> extint0;

/* -------------------------------------------------------------------------- */
/* Pin change interrupt                                                       */
/* -------------------------------------------------------------------------- */
#include "pins/pc_interrupt.hh"

typedef bit<gimsk, PCIE> pcie;
typedef PCIntPort<pcmsk, 0> pcint0;

/* -------------------------------------------------------------------------- */
/* Port definitions                                                           */
/* -------------------------------------------------------------------------- */
#include "pins/port.hh"

typedef Port<pinb, ddrb, portb, pcint0> PortB;

/* -------------------------------------------------------------------------- */
/* Pin definitions                                                            */
/* -------------------------------------------------------------------------- */
#include "pins/pin.hh"

typedef Pin<PortB, 0, oc0a, oc1a, extint0> pin0;
typedef Pin<PortB, 1, oc0b, oc1a>          pin1;
typedef Pin<PortB, 2, adc1>                pin2;
typedef Pin<PortB, 3, adc3, oc1b>          pin3;
typedef Pin<PortB, 4, adc2, oc1b>          pin4;
typedef Pin<PortB, 5, adc0>                pin5;

/* -- SPI ports ------------------------------------------------------------- */
typedef pin13 sck;
typedef pin12 miso;
typedef pin11 mosi;
typedef pin10 ss;
/* -- I2C ports ------------------------------------------------------------- */
typedef pinA5 scl;
typedef pinA4 sda;

/* -------------------------------------------------------------------------- */
/* Serial                                                                     */
/* -------------------------------------------------------------------------- */
#include "serial/hw_serial.hh"

typedef HWSerial<ucsr0a, ucsr0b, ucsr0c, ubrr0h, ubrr0l, udr0> Serial0;

/* -------------------------------------------------------------------------- */
/* SPI                                                                        */
/* -------------------------------------------------------------------------- */
#include "spi/hw_spi.hh"

template<typename _ss = ss>
class SPI : public HWSPI<sck, miso, mosi, _ss> {};

typedef SPI<pin10> SPI0;

#endif // CONFTN85_HH
