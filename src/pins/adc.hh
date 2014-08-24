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

#ifndef __ADC_HH__
#define __ADC_HH__

enum adc_prescaler_t {
  _adc_prescaler_2   = 0x0,
  _adc_prescaler_4   = 0x2,
  _adc_prescaler_8   = 0x3,
  _adc_prescaler_16  = 0x4,
  _adc_prescaler_32  = 0x5,
  _adc_prescaler_64  = 0x6,
  _adc_prescaler_128 = 0x7
};

enum adc_reference_t {
  _adc_ref_avref       = 0x0,
  _adc_ref_avcc        = 0x1,
  _adc_ref_internal    = 0x3
};

/* -------------------------------------------------------------------------- */
/* Analog to Digital convertion methods                                       */
/* -------------------------------------------------------------------------- */
template<uint8_t mux, typename adc_mux = admux,
         typename adc_ctrl_a = adcsra, typename adc_ctrl_b = adcsrb,
         typename adc_data = adcw>
class ADCPort { /* For 10bits resolution a clock between 50 and 200kHz is needed */
public:
  /// initialize the Analog to digital converter
  static void init(adc_prescaler_t prescaler = _adc_prescaler_128) {
    adc_ctrl_a::sbits<ADPS0, 0x03>(prescaler);
    // enable a2d conversions
    adc_ctrl_a::sbit(ADEN);
  }

  /// port is a value between 0 and 8
  static uint16_t read(adc_reference_t ref = _adc_ref_avcc) {
    // select the reference and the port to read
    adc_mux::sbits<REFS0, 0x03>(ref);
    adc_mux::sbits<MUX0, 0x0F>(mux);
#if defined(MUX5)
    adc_ctrl_b::sbits<MUX5, 0x01>(mux >> 5);
#endif

    // start the conversion
    adc_ctrl_a::sbit(ADSC);
    // ADSC is cleared when the conversion finishes
    adc_ctrl_a::wait_bit_is_clear(ADSC);

    if(adc_mux::getbit(ADLAR))
      return (adc_data::val() >> 6);
    else
      return (adc_data::val());
  }

  enum { type_id = _type_adc };
};

#endif //__ADC_HH__
