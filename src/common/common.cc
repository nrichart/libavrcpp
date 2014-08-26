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


#include "common/common.hh"

void init() {
  // this needs to be called before setup() or some functions won't
  // work there
  sei();

  // timers 1 and 2 are used for phase-correct hardware pwm
  // this is better for motors as it ensures an even waveform
  // note, however, that fast pwm mode can achieve a frequency of up
  // 8 MHz (with a 16 MHz clock) at 50% duty cycle

  // set timer 0 prescale factor to 64
  Timer0::setPrescaler(_timer_t0_clk_64);
  // configure timer 0 for phase correct pwm (8-bit)
  Timer0::setWaveform (_timer_t0_pwm_phase_corret_ff);

  // set timer 1 prescale factor to 64
  Timer1::setPrescaler(_timer_t0_clk_64);
  // put timer 1 in 8-bit phase correct pwm mode
  Timer1::setWaveform (_timer_t1_pwm_phase_corret_ff_8);

#if defined(TCCR2A) && !defined(__AVR_ATmega32U4__)
  // set timer 2 prescale factor to 64
  Timer2::setPrescaler(_timer_t2_clk_64);
  // configure timer 2 for phase correct pwm (8-bit)
  Timer2::setWaveform (_timer_t0_pwm_phase_corret_ff);
#endif

  // the bootloader connects pins 0 and 1 to the USART; disconnect them
  // here so they can be used as normal digital i/o; they will be
  // reconnected in Serial.begin()
#if defined(UCSRB)
  //UCSRB = 0;
#elif defined(UCSR0B)
  //UCSR0B = 0;
#endif
}
