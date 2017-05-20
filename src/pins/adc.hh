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
/* -------------------------------------------------------------------------- */

#ifndef ADC_HH
#define ADC_HH

/* -------------------------------------------------------------------------- */

namespace adc {
enum _prescaler_t {
  _prescaler_2   = 0x0,
  _prescaler_4   = 0x2,
  _prescaler_8   = 0x3,
  _prescaler_16  = 0x4,
  _prescaler_32  = 0x5,
  _prescaler_64  = 0x6,
  _prescaler_128 = 0x7
};

enum _reference_t {
  _ref_avref       = 0x0,
  _ref_avcc        = 0x1,
  _ref_internal    = 0x3
};
} // adc

namespace adc {
typedef adcsra::bit<ADSC> adsc;
typedef adcsra::bit<ADEN> aden;
typedef adcsra::bit<ADATE> adate;
typedef adcsra::bit<ADIF> adif;
typedef adcsra::bit<ADIE> adie;

typedef adcsra::bit<ADPS0> adps0;
typedef adcsra::bit<ADPS1> adps1;
typedef adcsra::bit<ADPS2> adps2;
typedef adcsra::bits<ADPS0, _or<adps0, adps1, adps2>::value> adcps;

typedef admux::bit<REFS0> refs0;
typedef admux::bit<REFS1> refs1;
typedef admux::bits<REFS0, _or<refs0, refs1>::value> adcrefs;

typedef admux::bit<ADLAR> adlar;

typedef admux::bit<MUX0> mux0;
typedef admux::bit<MUX1> mux1;
typedef admux::bit<MUX2> mux2;
typedef admux::bit<MUX3> mux3;
#if defined(MUX4)
typedef admux::bit<MUX4> mux4;
typedef admux::bits<MUX0, _or<mux0, mux1, mux2, mux3, mux4>::value> adcmux;
#else
typedef admux::bits<MUX0, _or<mux0, mux1, mux2, mux3>::value> adcmux;
#endif
#if defined(MUX5)
typedef adcsrb::bit<MUX5> mux5;
#endif
} // adc

/* -------------------------------------------------------------------------- */
/* Analog to Digital convertion methods                                       */
/* -------------------------------------------------------------------------- */
template<uint8_t mux>
class ADCPort { /* For 10bits resolution a clock between 50 and 200kHz is needed */
public:
  /// initialize the Analog to digital converter
  static void init(adc::_prescaler_t prescaler = adc::_prescaler_128) {
    adc::adcps::set(prescaler);
    // enable a2d conversions
    adc::aden::set();
  }

  /// port is a value between 0 and 8
  static uint16_t read(adc::_reference_t ref = adc::_ref_avcc) {
    // select the reference and the port to read
    adc::adcrefs::set(ref);
    adc::adcmux::set(mux);
#if defined(MUX5)
    if(mux >> 5) {
      adc::mux5::set();
    } else {
      adc::mux5::clear();
    }
#endif

    // start the conversion
    adc::adsc::set();
    // ADSC is cleared when the conversion finishes
    adc::adsc::wait_is_clear();

    if(adc::adlar::get())
      return (adcw::val() >> 6);
    else
      return (adcw::val());
  }

  enum { type_id = _type_adc };
};

#endif // ADC_HH
