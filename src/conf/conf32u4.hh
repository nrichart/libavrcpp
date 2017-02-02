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

#ifndef CONF32U4_HH
#define CONF32U4_HH

/*
  D0         PD2            RXD1/INT2
  D1         PD3            TXD1/INT3
  D2         PD1  SDA	    SDA/INT1
  D3~        PD0  PWM8/SCL  OC0B/SCL/INT0
  D4	A6   PD4            ADC8
  D5~        PC6  ???       OC3A/~OC4A
  D6~	A7   PD7  FastPWM   T0/OC4D/ADC10
  D7	     PE6	    INT6/AIN0
  D8	A8   PB4	    ADC11/PCINT4
  D9~	A9   PB5  PWM16     OC1A/~OC4B/ADC12/PCINT5
  D10~  A10  PB6  PWM16	    OC1B/ OC4B/ADC13/PCINT6
  D11~	     PB7  PWM8/16   OC0A/ OC1C/~RTS /PCINT7
  D12	A11  PD6            T1/~OC4D/ADC9
  D13~	     PC7  PWM10     CLK0/OC4A

  A0    D18  PF7            ADC7
  A1    D19  PF6	    ADC6
  A2    D20  PF5	    ADC5
  A3    D21  PF4	    ADC4
  A4    D22  PF1	    ADC1
  A5    D23  PF0	    ADC0

  New pins D14..D17 to map SPI port to digital pins
  D14        PB3            MISO/PCINT3
  D15        PB1            SCK/PCINT1
  D16        PB2            MOSI/PCINT2
  D17        PB0            RXLED/SS/PCINT0

  TXLED	     PD5            XCK1/~CTS
  RXLED	     PB0            SS/PCINT0
  HWB        PE2            HWB
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
typedef reg<0x0C + __SFR_OFFSET> pine;
typedef reg<0x0D + __SFR_OFFSET> ddre;
typedef reg<0x0E + __SFR_OFFSET> porte;
typedef reg<0x0F + __SFR_OFFSET> pinf;
typedef reg<0x10 + __SFR_OFFSET> ddrf;
typedef reg<0x11 + __SFR_OFFSET> portf;
typedef reg<0x15 + __SFR_OFFSET> tifr0;
typedef reg<0x16 + __SFR_OFFSET> tifr1;
typedef reg<0x17 + __SFR_OFFSET> tifr2;
typedef reg<0x18 + __SFR_OFFSET> tifr3;
typedef reg<0x19 + __SFR_OFFSET> tifr4;
typedef reg<0x1A + __SFR_OFFSET> tifr5;
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
typedef reg<0x29 + __SFR_OFFSET> pllcsr;
typedef reg<0x2A + __SFR_OFFSET> gpior1;
typedef reg<0x2B + __SFR_OFFSET> gpior2;
typedef reg<0x2C + __SFR_OFFSET> spcr;
typedef reg<0x2D + __SFR_OFFSET> spsr;
typedef reg<0x2E + __SFR_OFFSET> spdr;
typedef reg<0x30 + __SFR_OFFSET> acsr;
typedef reg<0x31 + __SFR_OFFSET> ocdr;
typedef reg<0x32 + __SFR_OFFSET> pllfrq;
typedef reg<0x33 + __SFR_OFFSET> smcr;
typedef reg<0x34 + __SFR_OFFSET> mcusr;
typedef reg<0x35 + __SFR_OFFSET> mcucr;
typedef reg<0x37 + __SFR_OFFSET> spmcsr;
typedef reg<0x3B + __SFR_OFFSET> rampz;
typedef reg<0x3C + __SFR_OFFSET> eind;
typedef reg<0x60> wdtcsr;
typedef reg<0x61> clkpr;
typedef reg<0x64> prr0;
typedef reg<0x65> prr1;
typedef reg<0x66> osccal;
typedef reg<0x67> rcctrl;
typedef reg<0x68> pcicr;
typedef reg<0x69> eicra;
typedef reg<0x6A> eicrb;
typedef reg<0x6B> pcmsk0;
//typedef reg<0x6C> pcmsk1;
//typedef reg<0x6D> pcmsk2;
typedef reg<0x6E> timsk0;
typedef reg<0x6F> timsk1;
//typedef reg<0x70> timsk2;
typedef reg<0x71> timsk3;
typedef reg<0x72> timsk4;
//typedef reg<0x73> timsk5;
#ifndef _ASSEMBLER_
typedef reg<0x78, uint16_t> adc;
#endif
typedef reg<0x78, uint16_t> adcw;
typedef reg<0x78> adcl;
typedef reg<0x79> adch;
typedef reg<0x7A> adcsra;
typedef reg<0x7B> adcsrb;
typedef reg<0x7C> admux;
typedef reg<0x7D> didr2;
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
typedef reg<0x8C, uint16_t> ocr1c;
typedef reg<0x8C> ocr1cl;
typedef reg<0x8D> ocr1ch;
typedef reg<0x90> tccr3a;
typedef reg<0x91> tccr3b;
typedef reg<0x92> tccr3c;
typedef reg<0x94, uint16_t> tcnt3;
typedef reg<0x94> tcnt3l;
typedef reg<0x95> tcnt3h;
typedef reg<0x96, uint16_t> icr3;
typedef reg<0x96> icr3l;
typedef reg<0x97> icr3h;
typedef reg<0x98, uint16_t> ocr3a;
typedef reg<0x98> ocr3al;
typedef reg<0x99> ocr3ah;
typedef reg<0x9A, uint16_t> ocr3b;
typedef reg<0x9A> ocr3bl;
typedef reg<0x9B> ocr3bh;
typedef reg<0x9C, uint16_t> ocr3c;
typedef reg<0x9C> ocr3cl;
typedef reg<0x9D> ocr3ch;
typedef reg<0x9E> uhcon;
typedef reg<0x9F> uhint;
typedef reg<0xA0> uhien;
typedef reg<0xA1> uhaddr;
typedef reg<0xA2, uint16_t> uhfnum;
typedef reg<0xA2> uhfnuml;
typedef reg<0xA3> uhfnumh;
typedef reg<0xA4> uhflen;
typedef reg<0xA5> upinrqx;
typedef reg<0xA6> upintx;
typedef reg<0xA7> upnum;
typedef reg<0xA8> uprst;
typedef reg<0xA9> upconx;
typedef reg<0xAA> upcfg0x;
typedef reg<0xAB> upcfg1x;
typedef reg<0xAC> upstax;
typedef reg<0xAD> upcfg2x;
typedef reg<0xAE> upienx;
typedef reg<0xAF> updatx;
typedef reg<0xB0> tccr2a;
typedef reg<0xB1> tccr2b;
typedef reg<0xB2> tcnt2;
typedef reg<0xB3> ocr2a;
typedef reg<0xB4> ocr2b;
typedef reg<0xB8> twbr;
typedef reg<0xB9> twsr;
typedef reg<0xBA> twar;
typedef reg<0xBB> twdr;
typedef reg<0xBC> twcr;
typedef reg<0xBD> twamr;
typedef reg<0xBE, uint16_t> tcnt4;
typedef reg<0xBE> tcnt4l;
typedef reg<0xBF> tcnt4h;
typedef reg<0xBF> tc4h;
typedef reg<0xC0> tccr4a;
typedef reg<0xC1> tccr4b;
typedef reg<0xC2> tccr4c;
typedef reg<0xC3> tccr4d;
typedef reg<0xC4> tccr4e;
typedef reg<0xC5> clksel0;
typedef reg<0xC6> clksel1;
typedef reg<0xC7> clksta;
typedef reg<0xC8> ucsr1a;
typedef reg<0xC9> ucsr1b;
typedef reg<0xCA> ucsr1c;
typedef reg<0xCC, uint16_t> ubrr1;
typedef reg<0xCC> ubrr1l;
typedef reg<0xCD> ubrr1h;
typedef reg<0xCE> udr1;
typedef reg<0xCF> ocr4a;
typedef reg<0xD0> ocr4b;
typedef reg<0xD1> ocr4c;
typedef reg<0xD2> ocr4d;
typedef reg<0xD4> dt4;
typedef reg<0xD7> uhwcon;
typedef reg<0xD8> usbcon;
typedef reg<0xD9> usbsta;
typedef reg<0xDA> usbint;
typedef reg<0xDD> otgcon;
typedef reg<0xDE> otgien;
typedef reg<0xDF> otgint;
typedef reg<0xE0> udcon;
typedef reg<0xE1> udint;
typedef reg<0xE2> udien;
typedef reg<0xE3> udaddr;
typedef reg<0xE4, uint16_t> udfnum;
typedef reg<0xE4> udfnuml;
typedef reg<0xE5> udfnumh;
typedef reg<0xE6> udmfn;
typedef reg<0xE7> udtst;
typedef reg<0xE8> ueintx;
typedef reg<0xE9> uenum;
typedef reg<0xEA> uerst;
typedef reg<0xEB> ueconx;
typedef reg<0xEC> uecfg0x;
typedef reg<0xED> uecfg1x;
typedef reg<0xEE> uesta0x;
typedef reg<0xEF> uesta1x;
typedef reg<0xF0> ueienx;
typedef reg<0xF1> uedatx;
typedef reg<0xF2, uint16_t> uebcx;
typedef reg<0xF2> uebclx;
typedef reg<0xF3> uebchx;
typedef reg<0xF4> ueint;
typedef reg<0xF5> uperrx;
typedef reg<0xF6> upbclx;
typedef reg<0xF7> upbchx;
typedef reg<0xF8> upint;
typedef reg<0xF9> otgtcon;

/* -------------------------------------------------------------------------- */
/* ADC definitions                                                            */
/* -------------------------------------------------------------------------- */
#include "pins/adc.hh"
typedef ADCPort<0x00> adc0;
typedef ADCPort<0x01> adc1;
typedef ADCPort<0x04> adc4;
typedef ADCPort<0x05> adc5;
typedef ADCPort<0x06> adc6;
typedef ADCPort<0x07> adc7;
typedef ADCPort<0x27> adctemp;
typedef ADCPort<0x1E> adc1v1;
typedef ADCPort<0x1F> adcgnd;
typedef ADCPort<0x20> adc8;
typedef ADCPort<0x21> adc9;
typedef ADCPort<0x22> adc10;
typedef ADCPort<0x23> adc11;
typedef ADCPort<0x24> adc12;
typedef ADCPort<0x25> adc13;


/* -------------------------------------------------------------------------- */
/* Timers                                                                     */
/* -------------------------------------------------------------------------- */
#include "pins/timer.hh"

typedef timer_prescaler_type_1_t timer0_prescaler_t;
typedef timer_prescaler_type_1_t timer1_prescaler_t;
typedef timer_prescaler_type_1_t timer3_prescaler_t;
typedef timer_prescaler_type_3_t timer4_prescaler_t;

typedef TimerCounterControl<timer0_prescaler_t, timer_8bit_waveform_t,
                            tccr0a, tccr0b>         TCC0;
typedef TimerCounterControl<timer1_prescaler_t, timer_10bit_waveform_t,
                            tccr1a, tccr1b, tccr1c> TCC1;
typedef TimerCounterControl<timer3_prescaler_t, timer_10bit_waveform_t,
                            tccr3a, tccr3b, tccr3c> TCC3;
typedef TimerCounterControl<timer4_prescaler_t, timer_hs_waveform_t,
                            tccr4a, tccr4b, tccr4c,
                            tccr4d, tccr4e, true>   TCC4;

typedef OutputCompare<TCC0, ocr0a, ocr0b> OC0;
typedef OutputCompare<TCC1, ocr1a, ocr1b> OC1;
typedef OutputCompare<TCC3, ocr3a, ocr3b> OC3;
typedef OutputCompare<TCC4, ocr4a, ocr4b, ocr4d> OC4;

typedef bit<timsk0, TOIE0> toie0;
typedef bit<timsk1, TOIE1> toie1;
typedef bit<timsk3, TOIE3> toie3;
typedef bit<timsk4, TOIE4> toie4;


typedef Timer<OC0, tcnt0, toie0, 0> Timer0;
typedef Timer<OC1, tcnt1, toie1, 1> Timer1;
typedef Timer<OC3, tcnt3, toie3, 3> Timer3;
typedef Timer<OC4, tcnt4, toie4, 4> Timer4;

typedef TimerChannel<Timer0, _timer_channel_a> oc0a;
typedef TimerChannel<Timer0, _timer_channel_b> oc0b;
typedef TimerChannel<Timer1, _timer_channel_a> oc1a;
typedef TimerChannel<Timer1, _timer_channel_b> oc1b;
typedef TimerChannel<Timer3, _timer_channel_a> oc3a;
typedef TimerChannel<Timer3, _timer_channel_b> oc3b;
typedef TimerChannel<Timer4, _timer_channel_a> oc4a;
typedef TimerChannel<Timer4, _timer_channel_b> oc4b;
typedef TimerChannel<Timer4, _timer_channel_c> oc4c;
typedef TimerChannel<Timer4, _timer_channel_d> oc4d;


/* -------------------------------------------------------------------------- */
/* External Interrupt                                                         */
/* -------------------------------------------------------------------------- */
#include "pins/ext_interrupt.hh"

typedef  bit<eimsk, INT0> int0;
typedef  bit<eimsk, INT1> int1;
typedef  bit<eimsk, INT2> int2;
typedef  bit<eimsk, INT3> int3;
typedef  bit<eimsk, INT6> int6;
typedef  bits<eicra, ISC00, 0x3> isc0x;
typedef  bits<eicra, ISC10, 0x3> isc1x;
typedef  bits<eicra, ISC20, 0x3> isc2x;
typedef  bits<eicra, ISC30, 0x3> isc3x;
typedef  bits<eicrb, ISC60, 0x3> isc6x;

typedef ExtInterrupt<int0, isc0x> exint0;
typedef ExtInterrupt<int1, isc1x> exint1;
typedef ExtInterrupt<int2, isc2x> exint2;
typedef ExtInterrupt<int3, isc3x> exint3;
typedef ExtInterrupt<int6, isc6x> exint6;

/* -------------------------------------------------------------------------- */
/* Pin change interrupt                                                       */
/* -------------------------------------------------------------------------- */
#include "pins/pc_interrupt.hh"

typedef bit<pcicr, PCIE0> pcie0;

typedef PCIntPort<pcmsk0, pcie0> pcint0;

/* -------------------------------------------------------------------------- */
/* Port definitions                                                           */
/* -------------------------------------------------------------------------- */
#include "pins/port.hh"

typedef Port<pinb, ddrb, portb, pcint0> PortB;
typedef Port<pinc, ddrc, portc> PortC;
typedef Port<pind, ddrd, portd> PortD;
typedef Port<pine, ddre, porte> PortE;
typedef Port<pinf, ddrf, portf> PortF;

/* -------------------------------------------------------------------------- */
/* Pin definitions                                                            */
/* -------------------------------------------------------------------------- */
#include "pins/pin.hh"

typedef Pin<PortD, 2, exint2>      pin0;
typedef Pin<PortD, 3, exint3>      pin1;
typedef Pin<PortD, 1, exint1>      pin2;
typedef Pin<PortD, 0, exint0, oc0b>  pin3;
typedef Pin<PortD, 4, adc8>        pin4;
typedef Pin<PortC, 6, oc3a>        pin5;
typedef Pin<PortD, 7, adc10, oc4d> pin6;
typedef Pin<PortE, 6, exint6>      pin7;
typedef Pin<PortB, 4, adc11>       pin8;
typedef Pin<PortB, 5, adc12, oc1a> pin9;
typedef Pin<PortB, 6, adc13, oc1b> pin10;
typedef Pin<PortB, 7, oc0a>        pin11;
typedef Pin<PortD, 6, adc9>        pin12;
typedef Pin<PortC, 7, oc4a>        pin13;
typedef Pin<PortB, 3>              pin14;
typedef Pin<PortB, 1>              pin15;
typedef Pin<PortB, 2>              pin16;
typedef Pin<PortB, 0>              pin17;
typedef Pin<PortF, 7, adc7>        pinA0;
typedef Pin<PortF, 6, adc6>        pinA1;
typedef Pin<PortF, 5, adc5>        pinA2;
typedef Pin<PortF, 4, adc4>        pinA3;
typedef Pin<PortF, 1, adc1>        pinA4;
typedef Pin<PortF, 0, adc0>        pinA5;
typedef pin4                       pinA6;
typedef pin6                       pinA7;
typedef pin8                       pinA8;
typedef pin9                       pinA9;
typedef pin10                      pinA10;
typedef pin12                      pinA11;

/* -------------------------------------------------------------------------- */
/* Serial                                                                     */
/* -------------------------------------------------------------------------- */
#include "serial/hw_serial.hh"

typedef HWSerial<ucsr1a, ucsr1b, ucsr1c, ubrr1h, ubrr1l, udr1> Serial1;
typedef Serial1 Serial0; /// \todo code the usb interface
/* -------------------------------------------------------------------------- */
/* SPI                                                                        */
/* -------------------------------------------------------------------------- */
#include "spi/hw_spi.hh"

typedef HWSPI<pin15, pin14, pin16, pin17> SPI0;

#endif // CONF32U4_HH
