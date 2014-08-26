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
#include "common/unused.hh"

#include "pins/timer_config.hh"
#include "pins/registers.hh"
/* -------------------------------------------------------------------------- */

#ifndef __TIMER_HH__
#define __TIMER_HH__

#define WGMn0  0
#define WGMn2  3
#define COMnB0 2
#define COMnD0 2

enum timer_channel_t {
  _timer_channel_a = 2,
  _timer_channel_b = 1,
  _timer_channel_c = 0,
  _timer_channel_d = 4
};


/* -------------------------------------------------------------------------- */
/* Timer mode                                                                 */
/* -------------------------------------------------------------------------- */
enum timer_mode_t {   // non-PWM
  _timer_normal = 0,  /* normal port operation */
  _timer_toggle = 1,  /* toggle OCXX on match compare */
  _timer_clear  = 2,  /* clear OCXX on match compare
                            fast PWM: set at BOTTOM
                            phase correct PWM: at up counting, set at down counting
                      */
  _timer_set    = 3   /* set OCXX on match compare
                            fast PWM: clear at BOTTOM
                            phase correct PWM: at up counting, clear at down counting
                      */
};

/* -------------------------------------------------------------------------- */
/* Configuration                                                              */
/* -------------------------------------------------------------------------- */
template<typename _prescaler_t,
         typename _waveform_t,
         typename tccra, typename tccrb, typename tccrc = unused_type,
         typename tccrd = unused_type, typename tccre = unused_type,
         bool high_speed = false>
class TimerCounterControl {
public:
  typedef _prescaler_t prescaler_t;
  typedef _waveform_t waveform_t;

  typedef tccra reg_a;
  typedef tccrb reg_b;
  typedef tccrc reg_c;

  static inline void setWaveform(waveform_t waveform) {
    tccra::template sbits<WGMn0, 0x3>(waveform     );
    tccrb::template sbits<WGMn2, 0x3>(waveform >> 2);
  }

  static inline void setPrescaler(prescaler_t prescaler)
  { tccrb::template sbits<0, 0x07>(prescaler); }
};

/* -------------------------------------------------------------------------- */
template<typename _prescaler_t,
         typename _waveform_t,
         typename tccra, typename tccrb, typename tccrc,
         typename tccrd, typename tccre>
class TimerCounterControl<_prescaler_t, _waveform_t,
                          tccra, tccrb, tccrc, tccrd, tccre, true> {
public:
  typedef _prescaler_t prescaler_t;
  typedef _waveform_t waveform_t;

  static inline void setWaveform(waveform_t waveform) {
    tccrd::template sbits<0, 0x3>(waveform);
    tccrb::template sbits<7, 0x1>(waveform >> 2);
  }

  static inline void setPrescaler(prescaler_t prescaler)
  { tccrb::template sbits<0, 0xF>(prescaler); }
};


/* -------------------------------------------------------------------------- */
/* Output                                                                     */
/* -------------------------------------------------------------------------- */
template<typename oc, timer_channel_t channel>
struct OutputCompareModeHelper {
  static void setMode(timer_mode_t mode) {
    oc::tcc_regs::reg_a::template sbits<COMnB0 + channel * 2, 0x03>(mode);
  }
};

template<typename oc>
struct OutputCompareModeHelper<oc, _timer_channel_d> {
  static void setMode(timer_mode_t mode) {
    oc::tcc_regs::reg_c::template sbits<COMnD0, 0x03>(mode);
  }
};

template<typename tcc, typename channel_a, typename channel_b,
         typename channel_c = unused_type, typename channel_d = unused_type>
class OutputCompare {
public:
  typedef typename channel_a::type type;
  typedef channel_a a;
  typedef channel_b b;
  typedef channel_c c;
  typedef channel_d d;
  typedef OutputCompare<tcc, a, b, c, d> oc;
  typedef tcc tcc_regs;

  template<timer_channel_t channel>
  static inline void setMode(timer_mode_t mode)
  { OutputCompareModeHelper<oc, channel>::setMode(mode); }
};

/* -------------------------------------------------------------------------- */
/* Timers                                                                     */
/* -------------------------------------------------------------------------- */
template<typename oc, typename channel>
struct TimerSetOCHelper {
  static inline void setOC(typename oc::type val) { channel::set(val); }
};

template<typename oc>
struct TimerSetOCHelper<oc, unused_type> {
  static inline void setOC(typename oc::type val) {  }
};


typedef void (*user_ovf_t)(void);

/* -------------------------------------------------------------------------- */
template<typename oc_regs,
         typename tcnt,
         typename timsk, typename tifr, uint8_t id,
         bool as_pwm = true,
         bool as_async = false>
class Timer {
public:
  typedef typename oc_regs::type oc_type;
  typedef typename oc_regs::tcc_regs tcc_regs;
  typedef typename tcc_regs::prescaler_t prescaler_t;
  typedef typename tcc_regs::waveform_t  waveform_t;

  static inline void setWaveform(waveform_t waveform)
  { tcc_regs::setWaveform(waveform); }

  static inline void setPrescaler(prescaler_t prescaler)
  { tcc_regs::setPrescaler(prescaler); }

  template<timer_channel_t channel>
  static inline void setMode(timer_mode_t mode)
  { oc_regs::template setMode<channel>(mode); }

  template<timer_channel_t channel>
  static inline void setOC(typename oc_regs::type val) {
    switch(channel) {
    case _timer_channel_a: TimerSetOCHelper<oc_regs, typename oc_regs::a>::setOC(val); break;
    case _timer_channel_b: TimerSetOCHelper<oc_regs, typename oc_regs::b>::setOC(val); break;
    case _timer_channel_c: TimerSetOCHelper<oc_regs, typename oc_regs::c>::setOC(val); break;
    case _timer_channel_d: TimerSetOCHelper<oc_regs, typename oc_regs::d>::setOC(val); break;
    }
  }

  static void enableOverflowInterrupt(user_ovf_t ovf) {
    timsk::sbit(0);
    user_ovf = ovf;

  }

  static void onOVF() {
    if(user_ovf) (*user_ovf)();
  }

private:
  static user_ovf_t user_ovf;
};

/* -------------------------------------------------------------------------- */
/* Timer port connection                                                      */
/* -------------------------------------------------------------------------- */
template<typename timer, timer_channel_t channel>
class TimerChannel {
public:
  typedef typename timer::prescaler_t prescaler_t;
  typedef typename timer::waveform_t waveform_t;

  static inline void setWaveform(waveform_t waveform)
  { timer::setWavefrom(waveform); }
  static inline void setPrescaler(prescaler_t prescaler)
  { timer::setPrescaler(prescaler); }
  static inline void setMode(timer_mode_t mode)
  { timer::template setMode<channel>(mode); }
  static inline void setOC(typename timer::oc_type val)
  { timer::template setOC<channel>(val); }

  static inline void enableOverflowInterrupt(user_ovf_t  ovf)
  { timer::enableOverflowInterupt(ovf); }

  enum { type_id = _type_timer_channel };
};



template<typename oc_regs, typename tcnt,
         typename timsk, typename tifr, uint8_t id,
         bool as_pwm, bool as_async>
user_ovf_t Timer<oc_regs, tcnt, timsk, tifr, id, as_pwm, as_async>::user_ovf = NULL;

#endif /* __TIMER_HH__ */
