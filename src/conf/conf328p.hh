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

#ifndef CONF328P_HH
#define CONF328P_HH

/*
                                    +-\/-+
RESET / PCINT14               PC6  1|    |28  PC5 (AI5)  ADC5 / PCINT13 / SCL
  RXD / PCINT16          (D0) PD0  2|    |27  PC4 (AI4)  ADC4 / PCINT12 / SDA
  TXD / PCINT17	         (D1) PD1  3|    |26  PC3 (AI3)  ADC3 / PCINT11
 INT0 / PCINT18	         (D2) PD2  4|    |25  PC2 (AI2)  ADC2 / PCINT10
 INT1 / PCINT19 / OC2B  +(D3) PD3  5|    |24  PC1 (AI1)  ADC1 / PCINT9
  XCK / PCINT20 / T0     (D4) PD4  6|    |23  PC0 (AI0)  ADC0 / PCINT8
                              VCC  7|    |22  GND
                              GND  8|    |21  AREF
XTAL1 / PCINT6  / TOSC1       PB6  9|    |20  AVCC
XTAL2 / PCINT7  / TOSC2       PB7 10|    |19  PB5 (D13)         PCINT5  / SCK
   T1 / PCINT21 / OC0B  +(D5) PD5 11|    |18  PB4 (D12)         PCINT4  / MISO
 AIN0 / PCINT22 / OC0A  +(D6) PD6 12|    |17  PB3 (D11)+ OC2A / PCINT3  / MOSI
 AIN1 / PCINT23          (D7) PD7 13|    |16  PB2 (D10)+ OC1B / PCINT2  / SS
 ICP1 / PCINT0  / CLKO   (D8) PB0 14|    |15  PB1 (D9)+  OC1A / PCINT1
                                    +----+
*/

/* -------------------------------------------------------------------------- */
/* Registers definitions                                                      */
/* -------------------------------------------------------------------------- */
#include "pins/registers.hh"

typedef reg_io<0x03> pinb;
typedef reg_io<0x04> ddrb;
typedef reg_io<0x05> portb;
typedef reg_io<0x06> pinc;
typedef reg_io<0x07> ddrc;
typedef reg_io<0x08> portc;
typedef reg_io<0x09> pind;
typedef reg_io<0x0A> ddrd;
typedef reg_io<0x0B> portd;
typedef reg_io<0x15> tifr0;
typedef reg_io<0x16> tifr1;
typedef reg_io<0x17> tifr2;
typedef reg_io<0x1B> pcifr;
typedef reg_io<0x1C> eifr;
typedef reg_io<0x1D> eimsk;
typedef reg_io<0x1E> gpior0;
typedef reg_io<0x1F> eecr;
typedef reg_io<0x20> eedr;
typedef reg_io<0x21, uint16_t> eear;
typedef reg_io<0x21> eearl;
typedef reg_io<0x22> eearh;
typedef reg_io<0x23> gtccr;
typedef reg_io<0x24> tccr0a;
typedef reg_io<0x25> tccr0b;
typedef reg_io<0x26> tcnt0;
typedef reg_io<0x27> ocr0a;
typedef reg_io<0x28> ocr0b;
typedef reg_io<0x2A> gpior1;
typedef reg_io<0x2B> gpior2;
typedef reg_io<0x2C> spcr;
typedef reg_io<0x2D> spsr;
typedef reg_io<0x2E> spdr;
typedef reg_io<0x30> acsr;
typedef reg_io<0x33> smcr;
typedef reg_io<0x34> mcusr;
typedef reg_io<0x35> mcucr;
typedef reg_io<0x37> spmcsr;
typedef reg<0x60> wdtcsr;
typedef reg<0x61> clkpr;
typedef reg<0x64> prr;
typedef reg<0x66> osccal;
typedef reg<0x68> pcicr;
typedef reg<0x69> eicra;
typedef reg<0x6B> pcmsk0;
typedef reg<0x6C> pcmsk1;
typedef reg<0x6D> pcmsk2;
typedef reg<0x6E> timsk0;
typedef reg<0x6F> timsk1;
typedef reg<0x70> timsk2;
typedef reg<0x78, uint16_t> adc;
typedef reg<0x78, uint16_t> adcw;
typedef reg<0x78> adcl;
typedef reg<0x79> adch;
typedef reg<0x7A> adcsra;
typedef reg<0x7B> adcsrb;
typedef reg<0x7C> admux;
typedef reg<0x7E> didr0;
typedef reg<0x7F> didr1;
typedef reg<0x80> tccr1a;
typedef reg<0x81> tccr1b;
typedef reg<0x82> tccr1c;
typedef reg<0x84, uint16_t> tcnt1;
typedef reg<0x84> tcnt1l;
typedef reg<0x85> tcnt1h;
typedef reg<0x86, uint16_t> icr1;
typedef reg<0x86> icr1l;
typedef reg<0x87> icr1h;
typedef reg<0x88, uint16_t> ocr1a;
typedef reg<0x88> ocr1al;
typedef reg<0x89> ocr1ah;
typedef reg<0x8A, uint16_t> ocr1b;
typedef reg<0x8A> ocr1bl;
typedef reg<0x8B> ocr1bh;
typedef reg<0xB0> tccr2a;
typedef reg<0xB1> tccr2b;
typedef reg<0xB2> tcnt2;
typedef reg<0xB3> ocr2a;
typedef reg<0xB4> ocr2b;
typedef reg<0xB6> assr;
typedef reg<0xB8> twbr;
typedef reg<0xB9> twsr;
typedef reg<0xBA> twar;
typedef reg<0xBB> twdr;
typedef reg<0xBC> twcr;
typedef reg<0xBD> twamr;
typedef reg<0xC0> ucsr0a;
typedef reg<0xC1> ucsr0b;
typedef reg<0xC2> ucsr0c;
typedef reg<0xC4, uint16_t> ubrr0;
typedef reg<0xC4> ubrr0l;
typedef reg<0xC5> ubrr0h;
typedef reg<0xC6> udr0;

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
typedef ADCPort<0x04> adc4;
typedef ADCPort<0x05> adc5;
typedef ADCPort<0x06> adc6;
typedef ADCPort<0x07> adc7;
typedef ADCPort<0x08> adctemp;
typedef ADCPort<0x0E> adc1v1;
typedef ADCPort<0x0F> adcgnd;

/* -------------------------------------------------------------------------- */
/* Timers                                                                     */
/* -------------------------------------------------------------------------- */
#define TIMER0
#define TIMER1
#define TIMER2

typedef bit<timsk0, TOIE0> toie0;
typedef bit<timsk1, TOIE1> toie1;
typedef bit<timsk2, TOIE2> toie2;

#include "pins/timer_config.hh"

/* -------------------------------------------------------------------------- */
/* External Interrupt                                                         */
/* -------------------------------------------------------------------------- */
typedef bit<eimsk, INT0> int0;
typedef bit<eimsk, INT1> int1;
typedef bits<eicra, ISC00, 0x3> isc0x;
typedef bits<eicra, ISC10, 0x3> isc1x;

#define EXT_INT0
#define EXT_INT1

#include "pins/ext_interrupt.hh"

/* -------------------------------------------------------------------------- */
/* Pin change interrupt                                                       */
/* -------------------------------------------------------------------------- */
#include "pins/pc_interrupt.hh"

typedef bit<pcicr, PCIE0> pcie0;
typedef bit<pcicr, PCIE1> pcie1;
typedef bit<pcicr, PCIE2> pcie2;

typedef PCIntPort<pcmsk0, pcie0> pcint0;
typedef PCIntPort<pcmsk1, pcie1> pcint1;
typedef PCIntPort<pcmsk2, pcie2> pcint2;

/* -------------------------------------------------------------------------- */
/* Port definitions                                                           */
/* -------------------------------------------------------------------------- */
#include "pins/port.hh"

typedef Port<pinb, ddrb, portb, pcint0> PortB;
typedef Port<pinc, ddrc, portc, pcint1> PortC;
typedef Port<pind, ddrd, portd, pcint2> PortD;

/* -------------------------------------------------------------------------- */
/* Pin definitions                                                            */
/* -------------------------------------------------------------------------- */
#include "pins/pin.hh"

typedef Pin<PortD, 0> pin0;
typedef Pin<PortD, 1> pin1;
typedef Pin<PortD, 2, exint0> pin2;
typedef Pin<PortD, 3, oc2b, exint1> pin3;
typedef Pin<PortD, 4> pin4;
typedef Pin<PortD, 5, oc0b> pin5;
typedef Pin<PortD, 6, oc0a> pin6;
typedef Pin<PortD, 7> pin7;
typedef Pin<PortB, 0> pin8;
typedef Pin<PortB, 1, oc1a> pin9;
typedef Pin<PortB, 2, oc1b> pin10;
typedef Pin<PortB, 3, oc2a> pin11;
typedef Pin<PortB, 4> pin12;
typedef Pin<PortB, 5> pin13;
typedef Pin<PortC, 0, adc0> pinA0;
typedef Pin<PortC, 1, adc1> pinA1;
typedef Pin<PortC, 2, adc2> pinA2;
typedef Pin<PortC, 3, adc3> pinA3;
typedef Pin<PortC, 4, adc4> pinA4;
typedef Pin<PortC, 5, adc5> pinA5;

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
#include "serial.hh"

typedef HWSerial<ucsr0a, ucsr0b, ucsr0c, ubrr0h, ubrr0l, udr0> Serial0;

/* -------------------------------------------------------------------------- */

#include "twi/twi.hh"
#include "spi/spi.hh"


#endif // CONF328P_HH
