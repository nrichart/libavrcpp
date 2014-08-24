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

#include "pins/timer.hh"

#ifndef PIN_HH
#define PIN_HH

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Analog Read/Write Helper                                                   */
/* -------------------------------------------------------------------------- */
template<typename pin,
         typename T1 = unused_type, typename T2 = unused_type, typename T3 = unused_type,
         typename timer = typename select_type<T1, T2, T3, _type_timer_channel>::type >
struct PinAnalogWriteHelper {
  static inline void write(uint8_t val) {
    if (val == 0) pin::low();
    else if (val == 255) pin::high();
    else {
      timer::setMode(_timer_clear);
      timer::setOC(val);
    }
  }
};

template<typename pin, typename T1, typename T2, typename T3>
struct PinAnalogWriteHelper<pin, T1, T2, T3, unused_type> {
  static inline void write(uint8_t val) {
    if(val > 127) pin::high();
    else pin::low();
  }
};

template<typename pin,
         typename T1 = unused_type, typename T2 = unused_type, typename T3 = unused_type,
         typename adc = typename select_type<T1, T2, T3, _type_adc>::type >
struct PinAnalogReadHelper {
  static inline uint16_t read(adc_reference_t ref) {
    return adc::read(ref);
  }
};

template<typename pin, typename T1, typename T2, typename T3>
struct PinAnalogReadHelper<pin, T1, T2, T3, unused_type> {
  static inline uint16_t read(adc_reference_t ref) {
    if(pin::read() == HIGH) return 1024;
    else return 0;
  }
};

/* -------------------------------------------------------------------------- */
/* Pin                                                                        */
/* -------------------------------------------------------------------------- */
template<typename Port, uint8_t _bit,
         typename T1 = unused_type, typename T2 = unused_type, typename T3 = unused_type>
class Pin : Port, public select_type<T1, T2, T3, _type_external_interrupt>::type,
  PCIntPin<typename Port::pcint, _bit> {
protected:
  typedef Pin<Port, _bit, T1, T2, T3> pin;

public:
  static inline void output() { Port::output(_bit); }
  static inline void input()  { Port::input(_bit);  }

  static inline void high() { Port::high(_bit); }
  static inline void low()  { Port::low(_bit);  }

  static inline uint8_t read() { return Port::read(_bit); }
  static inline void write(uint8_t value) { Port::write(value, _bit); }

  static inline uint16_t analogRead(adc_reference_t ref = _adc_ref_avcc) {
    return PinAnalogReadHelper<pin, T1, T2, T3>::write(ref);
  }

  static inline void analogWrite(uint8_t val) {
    PinAnalogWriteHelper<pin, T1, T2, T3>::write(val);
  }

  enum { bit = _bit  };
  typedef typename Port::port port;
};

/* -------------------------------------------------------------------------- */

#endif // PIN_HH
