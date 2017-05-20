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
#include "common/common.hh"
#include "pins/timer.hh"
#include "pins/adc.hh"
/* -------------------------------------------------------------------------- */

#ifndef PIN_HH
#define PIN_HH

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Analog Read/Write Helper                                                   */
/* -------------------------------------------------------------------------- */
namespace pin_helpers_ {
template<typename pin, typename T>
struct analog_writer_ {
  static inline void write(uint8_t val) {
    if(val > 127) pin::high();
    else pin::low();
  }
};

template<typename pin, typename timer, timer_channel_t channel>
struct analog_writer_<pin, TimerChannel<timer, channel>> {
  using _timer = TimerChannel<timer, channel>;
  static inline void write(uint8_t val) {
    if (val == 0) pin::low();
    else if (val == 255) pin::high();
    else {
      _timer::setMode(_timer_clear);
      _timer::setOC(val);
    }
  }
};

template<typename pin, typename T>
struct analog_reader_ {
  static inline uint8_t read(adc::_reference_t) {
    if(pin::read() == HIGH) return uint8_t(1024);
    else return 0;
  }
};

template<typename pin>
template<uint8_t mux>
struct analog_reader_<pin, ADCPort<mux>> {
  using _adc = ADCPort<mux>;
  static inline uint16_t read(adc::_reference_t ref) {
    return _adc::read(ref);
  }
};

template<typename pin, typename ...T>
struct analog_write {
  static inline void write(uint8_t val) {
    analog_writer_<pin, select_type_t<_type_timer_channel, T...>>::write(val);
  }
};

template<typename pin, typename ...T>
struct analog_read {
  static inline void write(uint8_t val) {
   return analog_reader_<pin, select_type_t<_type_adc, T...>>::write(val);
  }
};
}

/* -------------------------------------------------------------------------- */
/* Pin                                                                        */
/* -------------------------------------------------------------------------- */
template<typename Port, uint8_t _bit, typename ...T>
class Pin : Port {//, PCIntPin<typename Port::pcint, _bit> {
protected:
  typedef Pin<Port, _bit, T...> pin;

public:
  static inline void output() { Port::output(_bit); }
  static inline void input()  { Port::input(_bit);  }

  static inline void high() { Port::high(_bit); }
  static inline void low()  { Port::low(_bit);  }

  static inline uint8_t read() { return Port::read(_bit); }
  static inline void write(uint8_t value) { Port::write(value, _bit); }

  static inline uint16_t analogRead(adc::_reference_t ref = adc::_ref_avcc) {
    return pin_helpers_::analog_read<pin, T...>::write(ref);
  }

  static inline void analogWrite(uint8_t val) {
    pin_helpers_::analog_write<pin, T...>::write(val);
  }

  enum { bit = _bit  };
  typedef typename Port::port port;
};

/* -------------------------------------------------------------------------- */

#endif // PIN_HH
