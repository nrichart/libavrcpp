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

#ifndef PORT_HH
#define PORT_HH

/* -------------------------------------------------------------------------- */
template<typename pinx, typename ddrx, typename portx,
         typename _pcint = unused_type>
class Port  {
public:
  typedef _pcint pcint;

  static inline void output(uint8_t bit) { ddrx::sbit(bit); }
  static inline void input(uint8_t bit)  { ddrx::cbit(bit); }

  static inline uint8_t read(uint8_t bit) { return pinx::getbit(bit); }

  static inline void high(uint8_t bit) { portx::sbit(bit); }
  static inline void low(uint8_t bit)  { portx::cbit(bit); }

  static inline void write(uint8_t value,
                           uint8_t bit) {
    if(value == LOW) portx::cbit(bit);
    else portx::sbit(bit);
  }

  typedef portx port;
};

#endif // PORT_HH
