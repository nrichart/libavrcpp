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

#ifndef PC_INTERRUPT_HH
#define PC_INTERRUPT_HH

template<typename pcmskx, uint8_t pcie>
class PCIntPort {
public:
  static inline void PCIntOn(uint8_t pin) {
    pcicr::sbit(pcie);
    pcmskx::sbit(pin);
  }

  static inline void PCIntOff(uint8_t pin) {
    pcmskx::cbit(pin);
  }
};

template<typename pcintport, uint8_t pin_num>
struct PCIntPin {
  static int PCIntOn() {
    return pcintport::PCIntOn(pin_num);
  }
  static int PCIntOff() {
    return pcintport::PCIntOff(pin_num);
  }
};

template<uint8_t pin_num>
class PCIntPin<unused_type, pin_num> { };

#endif /* PC_INTERRUPT_HH */
