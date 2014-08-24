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


#ifndef __RGBLED_HH__
#define __RGBLED_HH__

#include "common/common.hh"
#include "common/color.hh"

template<typename red, typename green, typename blue, bool is_common_annode = true>
class RGBLed {
public:
  static void activate() {
    red  ::output();
    green::output();
    blue ::output();

    uint8_t value = is_common_annode ? LOW : HIGH;
    red  ::write(value);
    green::write(value);
    blue ::write(value);
  }

  template<class Color>
  static void setColor(const Color & color) {
    red  ::analogWrite(is_common_annode ? 255 - color.red()   : color.red()  );
    green::analogWrite(is_common_annode ? 255 - color.green() : color.green());
    blue ::analogWrite(is_common_annode ? 255 - color.blue()  : color.blue() );
  }
};

#endif //__RGBLED_HH__
