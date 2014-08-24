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

#ifndef __SHIFT_HH__
#define __SHIFT_HH__

#define LSBFIRST 0
#define MSBFIRST 1

template<class dataPin, class clockPin, uint8_t bitOrder = MSBFIRST, bool cpol = 0, bool cpha = 1>
class ShiftRegister {
public:
  static uint8_t in() {
    uint8_t value = 0;
    for (uint8_t i = 0; i < 8; ++i) {
      clockPin::high();
      if (bitOrder == LSBFIRST)
        value |= dataPin::read() << i;
      else
        value |= dataPin::read() << (7 - i);
      clockPin::low();
    }
    return value;
  }

  static void out(uint8_t value) {
    for (uint8_t i = 0; i < 8; i++)  {
      if (bitOrder == LSBFIRST)
        dataPin::write(!!(value & (1 << i)));
      else
        dataPin::write(!!(value & (1 << (7 - i))));

      clockPin::high();
      clockPin::low();
    }
  }
};

#endif //__SHIFT_HH__
