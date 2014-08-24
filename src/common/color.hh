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

#ifndef __COLOR_HH__
#define __COLOR_HH__

enum colorspace_t {
  _rgb,
  _yuv,
};

template<colorspace_t cs = _rgb>
class Color { };

template<>
class Color<_rgb> {
public:
  Color(uint8_t r, uint8_t g, uint8_t b) :
    r(r), g(g), b(b) {}

  inline uint8_t & red  () { return r; }
  inline uint8_t & green() { return g; }
  inline uint8_t & blue () { return b; }
  inline uint8_t red  () const { return r; }
  inline uint8_t green() const { return g; }
  inline uint8_t blue () const { return b; }
private:
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

#endif /* __COLOR_HH__ */
