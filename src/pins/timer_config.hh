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

enum timer_waveform_t4_t {            // Mode               | TOP   | update OCRn | TOV flag
  _timer_t4_normal            = 0x00, // Normal             | OCRnC | Immediate   | Top
  _timer_t4_fast_pwm          = 0x10, // Fast PWM           | OCRnC | Top         | Top
  _timer_t4_pwm_pha_fre_cor   = 0x11, // PWM Phase and
                                      //  frequency cor     | OCRnC | Bottom      | Bottom
  _timer_t4_pwm_single_slope  = 0x12, // PWM / Single slope | OCRnC | Top         | Top
  _timer_t4_pwm_dual_slope    = 0x13, // PWM / Dual slope   | OCRnC | Bottom      | Bottom
};

#endif // TIMER_CONFIG_HH
