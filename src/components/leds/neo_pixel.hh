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

#ifndef __NEO_PIXEL_HH__
#define __NEO_PIXEL_HH__

#include "common/color.hh"

enum ws281x_drive_freq_t {
  _ws281x_800_khz,
  _ws281x_400_khz
};

#include "ws281x_display_helper.hh"

template < typename pin, ws281x_drive_freq_t drive_freq = _ws281x_800_khz >
class WS281x {
public:
  static void activate() {
    pin::output();
    reset();
  }

  static void reset() {
    pin::low();
    _delay_us(50);
  }

  static void display(uint8_t * rgb, uint8_t nb_leds) {
    WS281xDisplayHelper<pin, drive_freq>::display(rgb, nb_leds);
  }
};

template < typename pin, uint8_t nb_leds, ws281x_drive_freq_t drive_freq = _ws281x_800_khz >
class NeoPixel {
protected:
  typedef WS281x<pin, drive_freq> driver;

public:
  static void activate() {
    brightness = 255;
    driver::activate();
  }

  static void setBrightness(uint8_t b) {
    uint8_t ob = brightness;
    brightness = b;

    uint8_t * colors = leds;
    for (int i = 0; i < 3*nb_leds; ++i) {
      *colors = ((uint16_t) *colors * (uint16_t) b) / ob;
    }
  }

  template<typename Color>
  static void setLEDColor(uint8_t led, const Color & color) {
    if(led >= nb_leds) return;

    uint8_t * tled = leds + led * 3;
    tled[1] = ((uint16_t) color.red()   * brightness) / 255;
    tled[0] = ((uint16_t) color.green() * brightness) / 255;
    tled[2] = ((uint16_t) color.blue()  * brightness) / 255;
  }

  static void display() {
    driver::display(leds, nb_leds);
  }

private:
  static uint8_t leds[3*nb_leds];
  static uint8_t brightness;
};

template<typename pin, uint8_t nb_leds, ws281x_drive_freq_t drive_freq>
uint8_t  NeoPixel<pin, nb_leds, drive_freq>::leds[3*nb_leds];

template<typename pin, uint8_t nb_leds, ws281x_drive_freq_t drive_freq>
uint8_t NeoPixel<pin, nb_leds, drive_freq>::brightness;


#endif /* __NEO_PIXEL_HH__ */
