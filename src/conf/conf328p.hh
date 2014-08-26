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

typedef reg<0x03 + __SFR_OFFSET> pinb;
typedef reg<0x04 + __SFR_OFFSET> ddrb;
typedef reg<0x05 + __SFR_OFFSET> portb;
typedef reg<0x06 + __SFR_OFFSET> pinc;
typedef reg<0x07 + __SFR_OFFSET> ddrc;
typedef reg<0x08 + __SFR_OFFSET> portc;
typedef reg<0x09 + __SFR_OFFSET> pind;
typedef reg<0x0A + __SFR_OFFSET> ddrd;
typedef reg<0x0B + __SFR_OFFSET> portd;
typedef reg<0x15 + __SFR_OFFSET> tifr0;
typedef reg<0x16 + __SFR_OFFSET> tifr1;
typedef reg<0x17 + __SFR_OFFSET> tifr2;
typedef reg<0x1B + __SFR_OFFSET> pcifr;
typedef reg<0x1C + __SFR_OFFSET> eifr;
typedef reg<0x1D + __SFR_OFFSET> eimsk;
typedef reg<0x1E + __SFR_OFFSET> gpior0;
typedef reg<0x1F + __SFR_OFFSET> eecr;
typedef reg<0x20 + __SFR_OFFSET> eedr;
typedef reg<0x21 + __SFR_OFFSET, uint16_t> eear;
typedef reg<0x21 + __SFR_OFFSET> eearl;
typedef reg<0x22 + __SFR_OFFSET> eearh;
typedef reg<0x23 + __SFR_OFFSET> gtccr;
typedef reg<0x24 + __SFR_OFFSET> tccr0a;
typedef reg<0x25 + __SFR_OFFSET> tccr0b;
typedef reg<0x26 + __SFR_OFFSET> tcnt0;
typedef reg<0x27 + __SFR_OFFSET> ocr0a;
typedef reg<0x28 + __SFR_OFFSET> ocr0b;
typedef reg<0x2A + __SFR_OFFSET> gpior1;
typedef reg<0x2B + __SFR_OFFSET> gpior2;
typedef reg<0x2C + __SFR_OFFSET> spcr;
typedef reg<0x2D + __SFR_OFFSET> spsr;
typedef reg<0x2E + __SFR_OFFSET> spdr;
typedef reg<0x30 + __SFR_OFFSET> acsr;
typedef reg<0x33 + __SFR_OFFSET> smcr;
typedef reg<0x34 + __SFR_OFFSET> mcusr;
typedef reg<0x35 + __SFR_OFFSET> mcucr;
typedef reg<0x37 + __SFR_OFFSET> spmcsr;
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
#include "pins/timer.hh"

typedef timer_prescaler_t0_t timer0_prescaler_t;
typedef timer_prescaler_t0_t timer1_prescaler_t;
typedef timer_prescaler_t2_t timer2_prescaler_t;

typedef TimerCounterControl<timer0_prescaler_t, timer_waveform_t0_t,
                            tccr0a, tccr0b>         TCC0;
typedef TimerCounterControl<timer1_prescaler_t, timer_waveform_t1_t,
                            tccr1a, tccr1b, tccr1c> TCC1;
typedef TimerCounterControl<timer2_prescaler_t, timer_waveform_t0_t,
                            tccr2a, tccr2b>         TCC2;

typedef OutputCompare<TCC0, ocr0a, ocr0b> OC0;
typedef OutputCompare<TCC1, ocr1a, ocr1b> OC1;
typedef OutputCompare<TCC2, ocr2a, ocr2b> OC2;

typedef Timer<OC0, tcnt0, timsk0, tifr0, 0> Timer0;
typedef Timer<OC1, tcnt1, timsk1, tifr1, 1> Timer1;
typedef Timer<OC2, tcnt2, timsk2, tifr2, 2> Timer2;

typedef TimerChannel<Timer0, _timer_channel_a> oc0a;
typedef TimerChannel<Timer0, _timer_channel_b> oc0b;
typedef TimerChannel<Timer1, _timer_channel_a> oc1a;
typedef TimerChannel<Timer1, _timer_channel_b> oc1b;
typedef TimerChannel<Timer2, _timer_channel_a> oc2a;
typedef TimerChannel<Timer2, _timer_channel_b> oc2b;

/* -------------------------------------------------------------------------- */
/* External Interrupt                                                         */
/* -------------------------------------------------------------------------- */
#include "pins/ext_interrupt.hh"

typedef ExtInterrupt<0> int0;
typedef ExtInterrupt<1> int1;

/* -------------------------------------------------------------------------- */
/* Pin change interrupt                                                       */
/* -------------------------------------------------------------------------- */
#include "pins/pc_interrupt.hh"

typedef PCIntPort<pcmsk0, 0> pcint0;
typedef PCIntPort<pcmsk1, 1> pcint1;
typedef PCIntPort<pcmsk2, 2> pcint2;

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
typedef Pin<PortD, 2, int0> pin2;
typedef Pin<PortD, 3, oc2b, int1> pin3;
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
#include "serial/hw_serial.hh"

typedef HWSerial<ucsr0a, ucsr0b, ucsr0c, ubrr0h, ubrr0l, udr0> Serial0;

/* -------------------------------------------------------------------------- */
/* SPI                                                                        */
/* -------------------------------------------------------------------------- */
#include "spi/hw_spi.hh"

template<typename _ss = ss>
class SPI : public HWSPI<sck, miso, mosi, _ss> {};

typedef SPI<pin10> SPI0;

#endif // CONF328P_HH
