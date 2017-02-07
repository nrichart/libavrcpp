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
#include "pins/timer.hh"
/* -------------------------------------------------------------------------- */

#ifndef TIMER_CONFIG_HH
#define TIMER_CONFIG_HH

/*
   - Prescaler
     Timer0 Timer1 Timer3 timer_prescaler_t0_t
     Timer2               timer_prescaler_t2_t
     Timer4               timer_prescaler_t4_t

   - Waveform
     Timer0 Timer2 Timer3 timer_waveform_t0_t
     Timer1               timer_waveform_t1_t
     Timer4               timer_waveform_t4_t
*/

/* -------------------------------------------------------------------------- */
/* Clock Prescaler                                                            */
/* -------------------------------------------------------------------------- */
/* Config for ATmega328P */
enum timer_prescaler_t0_t {
  _timer_t0_no_input = 0,
  _timer_t0_clk_1    = 1,
  _timer_t0_clk_8    = 2,
  _timer_t0_clk_64   = 3,
  _timer_t0_clk_256  = 4,
  _timer_t0_clk_1024 = 5,
  _timer_t0_ext_fe   = 6,
  _timer_t0_ext_re   = 7
};

typedef timer_prescaler_t0_t timer_prescaler_t1_t;
typedef timer_prescaler_t0_t timer_prescaler_t3_t;

/* -------------------------------------------------------------------------- */
enum timer_prescaler_t2_t {
  _timer_t2_no_input = 0,
  _timer_t2_clk_1    = 1,
  _timer_t2_clk_8    = 2,
  _timer_t2_clk_32   = 3,
  _timer_t2_clk_64   = 4,
  _timer_t2_clk_128  = 5,
  _timer_t2_clk_256  = 6,
  _timer_t2_clk_1024 = 7
};

/* -------------------------------------------------------------------------- */
enum timer_prescaler_t4_t {
  _timer_t4_no_input  = 0,
  _timer_t4_clk_1     = 1,
  _timer_t4_clk_2     = 2,
  _timer_t4_clk_4     = 3,
  _timer_t4_clk_8     = 4,
  _timer_t4_clk_16    = 5,
  _timer_t4_clk_32    = 6,
  _timer_t4_clk_64    = 7,
  _timer_t4_clk_128   = 8,
  _timer_t4_clk_256   = 9,
  _timer_t4_clk_512   = 10,
  _timer_t4_clk_1024  = 11,
  _timer_t4_clk_2048  = 12,
  _timer_t4_clk_4096  = 13,
  _timer_t4_clk_8192  = 14,
  _timer_t4_clk_16384 = 15
};

/* -------------------------------------------------------------------------- */
/* Waveform                                                                   */
/* -------------------------------------------------------------------------- */
enum timer_waveform_t0_t {             // Mode               | TOP   | update OCRn | TOV flag
  _timer_t0_normal                = 0, // Normal             | 0xFF  | Immediate   | Max
  _timer_t0_pwm_phase_corret_ff   = 1, // PWM, Phase Correct | 0xFF  | Top         | Bottom
  _timer_t0_ctc                   = 2, // CTC                | OCRnA | Immediate   | Max
  _timer_t0_fast_pwm_ff           = 3, // Fast PWM           | 0xFF  | Bottom      | Max
  _timer_t0_pwm_phase_corret_ocra = 5, // PWM, Phase Correct | 0xFF  | Top         | Bottom
  _timer_t0_fast_pwm_ocra         = 7  // Fast PWM           | OCRnA | Bottom      | Top
};

typedef timer_waveform_t0_t timer_waveform_t2_t;

/* -------------------------------------------------------------------------- */
enum timer_waveform_t1_t {             // Mode                 |  TOP   | up OCRx | TOV flag
  _timer_t1_normal                = 0, // Normal               | 0xFFFF | Immedia | Max
  _timer_t1_pwm_phase_corret_ff_8 = 1, // PWM, Phase Cor 8bit  | 0x00FF | Top     | Bottom
  _timer_t1_pwm_phase_corret_ff_9 = 2, // PWM, Phase Cor 9bit  | 0x01FF | Top     | Bottom
  _timer_t1_pwm_phase_corret_ff_10= 3, // PWM, Phase Cor 10bit | 0x03FF | Top     | Bottom
  _timer_t1_ctc_ocra              = 4, // CTC                  | OCRnA  | Immedia | Max
  _timer_t1_fast_pwm_ff_8         = 5, // Fast PWM 8bit        | 0x00FF | Bottom  | Max
  _timer_t1_fast_pwm_ff_9         = 6, // Fast PWM 9bit        | 0x01FF | Bottom  | Max
  _timer_t1_fast_pwm_ff_10        = 7, // Fast PWM 10bit       | 0x03FF | Bottom  | Max
  _timer_t1_pwm_pha_fre_cor_icr   = 8, // PWM Phase and
                                       //    frequency cor     | ICRn   | Bottom  | Bottom
  _timer_t1_pwm_pha_fre_cor_ocra  = 9, // PWM Phase and
                                       //    frequency cor     | OCRnA  | Bottom  | Bottom
  _timer_t1_pwm_phase_corret_icr  = 10,// PWM, Phase Cor       | ICRn   | Top     | Bottom
  _timer_t1_pwm_phase_corret_ocra = 11,// PWM, Phase Cor       | OCRnA  | Top     | Bottom
  _timer_t1_ctc_icr               = 12,// CTC                  | ICRn   | Immedia | Max
  _timer_t1_fast_pwm_icr          = 14,// Fast PWM             | ICRn   | Bottom  | Top
  _timer_t1_fast_pwm_ocra         = 15 // Fast PWM             | OCRnA  | Bottom  | Top
};

typedef timer_waveform_t1_t timer_waveform_t3_t;

/* -------------------------------------------------------------------------- */
enum timer_waveform_t4_t {            // Mode               | TOP   | update OCRn | TOV flag
  _timer_t4_normal            = 0x00, // Normal             | OCRnC | Immediate   | Top
  _timer_t4_fast_pwm          = 0x10, // Fast PWM           | OCRnC | Top         | Top
  _timer_t4_pwm_pha_fre_cor   = 0x11, // PWM Phase and
                                      //  frequency cor     | OCRnC | Bottom      | Bottom
  _timer_t4_pwm_single_slope  = 0x12, // PWM / Single slope | OCRnC | Top         | Top
  _timer_t4_pwm_dual_slope    = 0x13, // PWM / Dual slope   | OCRnC | Bottom      | Bottom
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#if defined(TIMER0)
typedef TimerCounterControl<timer_prescaler_t0_t, timer_waveform_t0_t, tccr0a, tccr0b> tcc0;

typedef OutputCompare<tcc0, ocr0a, ocr0b> oc0;

typedef Timer<oc0, tcnt0, toie0, 0> Timer0;

typedef TimerChannel<Timer0, _timer_channel_a> oc0a;
typedef TimerChannel<Timer0, _timer_channel_b> oc0b;
#endif

#if defined(TIMER1)
typedef TimerCounterControl<timer_prescaler_t1_t, timer_waveform_t1_t, tccr1a, tccr1b, tccr1c> tcc1;
typedef OutputCompare<tcc1, ocr1a, ocr1b> oc1;

typedef Timer<oc1, tcnt1, toie1, 1> Timer1;

typedef TimerChannel<Timer1, _timer_channel_a> oc1a;
typedef TimerChannel<Timer1, _timer_channel_b> oc1b;
#endif

#if defined(TIMER2)
typedef TimerCounterControl<timer_prescaler_t2_t, timer_waveform_t2_t, tccr2a, tccr2b> tcc2;

typedef OutputCompare<tcc2, ocr2a, ocr2b> oc2;

typedef Timer<oc2, tcnt2, toie2, 2> Timer2;

typedef TimerChannel<Timer2, _timer_channel_a> oc2a;
typedef TimerChannel<Timer2, _timer_channel_b> oc2b;
#endif

#if defined(TIMER3)
typedef TimerCounterControl<timer_prescaler_t3_t, timer_waveform_t3_t, tccr3a, tccr3b, tccr3c> tcc3;

typedef OutputCompare<tcc3, ocr3a, ocr3b> oc3;

typedef Timer<oc3, tcnt3, toie3, 3> Timer3;

typedef TimerChannel<Timer3, _timer_channel_a> oc3a;
typedef TimerChannel<Timer3, _timer_channel_b> oc3b;
#endif

#if defined(TIMER4)
typedef TimerCounterControl<timer_prescaler_t4_t, timer_waveform_t4_t, tccr4a, tccr4b, tccr4c, tccr4d, tccr4e, true> tcc4;

typedef OutputCompare<tcc4, ocr4a, ocr4b, ocr4d> oc4;

typedef Timer<oc4, tcnt4, toie4, 4> Timer4;

typedef TimerChannel<Timer4, _timer_channel_a> oc4a;
typedef TimerChannel<Timer4, _timer_channel_b> oc4b;
typedef TimerChannel<Timer4, _timer_channel_c> oc4c;
typedef TimerChannel<Timer4, _timer_channel_d> oc4d;
#endif
/* -------------------------------------------------------------------------- */



#endif // TIMER_CONFIG_HH
