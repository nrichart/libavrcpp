/**
   $Id:$

   ST7565 LCD library!

   Copyright (C) 2010 Limor Fried, Adafruit Industries

   This library is free software; you can redistribute it and/or modify it under
   the terms of the  GNU Lesser General Public License as  published by the Free
   Software Foundation; either  version 2.1 of the License, or  (at your option)
   any later version.

   This library is distributed  in the hope that it will  be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
   details.

   You should  have received  a copy  of the GNU  Lesser General  Public License
   along with this library; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/**
   some of this code was written by <cstone@pobox.com> originally; it is in the public domain.

   Modifications:
   - 08/2014 This code has been modified and cleaned for the need of libavrc++ by netWorms
*/

#ifndef __ST7565_HH__
#define __ST7565_HH__

#include "common/common.hh"
#include "components/leds/rgb_led.hh"

template<typename rst, typename a0, typename led,
         typename spi_master = SPI0>
class ST7565 {
public:
  enum { width  = 128 };
  enum { height = 64 };

  /* ------------------------------------------------------------------------ */
  static void activate() {
    //Reset the LCD to a known state
    rst::output();
    rst::low();
    _delay_ms(500);
    rst::high();

    //Configure control pins
    a0::output();

    led::activate();

    spi_master::activate();
    spi_master::setMode(_spi_mode_0);
    spi_master::setBitOrder(_spi_bo_msb);

    // LCD bias select
    writeCmd(_set_bias_7);
    // ADC select
    writeCmd(_set_adc_normal);
    // SHL select
    writeCmd(_set_com_normal);
    // Initial display line
    writeCmd(_set_disp_start_line);

    // turn on voltage converter (VC=1, VR=0, VF=0)
    writeCmd(_set_power_control | 0x4);
    // wait for 50% rising
    _delay_ms(50);

    // turn on voltage regulator (VC=1, VR=1, VF=0)
    writeCmd(_set_power_control | 0x6);
    // wait >=50ms
    _delay_ms(50);

    // turn on voltage follower (VC=1, VR=1, VF=1)
    writeCmd(_set_power_control | 0x7);
    // wait
    _delay_ms(10);

    // set lcd operating voltage (regulator resistor, ref voltage resistor)
    writeCmd(_set_resistor_ratio | 0x6);

    _delay_ms(10);

    writeCmd(_display_on);
    writeCmd(_set_allpts_normal);

    setBrightness(0x25);
    setBacklightColor(Color<_rgb>(255, 255, 255));

    for(uint8_t p = 0; p < 8; p++) {
      writeCmd(_set_page | p);
      for(uint8_t c = 0; c < 129; c++) {
	writeCmd(_set_column_lower | (c & 0x0F));
	writeCmd(_set_column_upper | ((c >> 4) & 0x0F));
	writeData(0x00);
      }
    }
  }

  template<class Color>
  static void setBacklightColor(const Color & color) {
    led::setColor(color);
  }

  static void setBrightness(uint8_t val) {
    writeCmd(_set_volume_first);
    writeCmd(val & 0x3f);
  }

  static void reverse(bool rev = true) {
    if(rev) {
      writeCmd(_set_disp_reverse);
    } else {
      writeCmd(_set_disp_normal);
    }
  }

protected:
  enum partial_update_t { partial_update = true };

  static void display(uint8_t * buffer, uint8_t x_min = 0, uint8_t y_min = 0,
		      uint8_t x_max = height - 1, uint8_t y_max = width - 1) {
    uint8_t col, maxcol, p;

    uint8_t pmax = (y_max / 8 + 1);
    pmax = pmax > 8 ? 8 : pmax;
    for(p = (y_min / 8); p < pmax; ++p) {
      writeCmd(_set_page | page_map[p]);

      // start at the beginning of the row
      col = x_min;
      maxcol = x_max;

      writeCmd(_set_column_lower | ((col+1) & 0xF));
      writeCmd(_set_column_upper | (((col+1) >> 4) & 0x0F));
      writeCmd(_rmw);

      spi_master::select();
      for(; col <= maxcol; col++) {
	writeData(buffer[(width*p)+col]);
      }
      spi_master::unselect();
    }
  }

  /* ------------------------------------------------------------------------ */
  /* Instruction set                                                          */
  /* ------------------------------------------------------------------------ */
private:
  enum command_t {
    _set_column_lower     = 0x00,
    _set_column_upper     = 0x10,
    _set_resistor_ratio   = 0x20,
    _set_power_control    = 0x28,
    _set_disp_start_line  = 0x40,
    _set_volume_first     = 0x81,
    _set_adc_normal       = 0xa0,
    _set_adc_reverse      = 0xa1,
    _set_bias_9           = 0xa2,
    _set_bias_7           = 0xa3,
    _set_allpts_normal    = 0xa4,
    _set_allpts_on        = 0xa5,
    _set_disp_normal      = 0xa6,
    _set_disp_reverse     = 0xa7,
    _set_static_off       = 0xac,
    _set_static_on        = 0xad,
    _display_off          = 0xae,
    _display_on           = 0xaf,
    _set_page             = 0xb0,
    _set_com_normal       = 0xc0,
    _set_com_reverse      = 0xc8,
    _rmw                  = 0xe0,
    _internal_reset       = 0xe2,
    _nop                  = 0xe3,
    _rmw_clear            = 0xee,
    _test                 = 0xf0,
    _set_booster_first    = 0xf8
  };

  enum booster_t {
    _booster_234 = 0,
    _booster_5   = 1,
    _booster_6   = 3
  };

  // frame buffer
  static uint8_t page_map[8];

  /* ------------------------------------------------------------------------ */
  static void writeCmd(uint8_t data) {
    //Tell the LCD that we are writing either to data or a command
    a0::low();
    //Send the data
    spi_master::transfer(data);
  }

  /* ------------------------------------------------------------------------ */
  static void writeData(uint8_t data) {
    //Tell the LCD that we are writing either to data or a command
    a0::high();
    //Send the data
    spi_master::transferByte(data);
  }
};


template<typename rst, typename a0, typename led,
         typename spi_master>
uint8_t ST7565<rst, a0, led, spi_master>::page_map[8] = { 3, 2, 1, 0, 7, 6, 5, 4 };

#endif // __ST7565_HH__
