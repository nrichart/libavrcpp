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

#ifndef __SPI_HH__
#define __SPI_HH__

template<typename _sck, typename _miso, typename _mosi, typename _ss,
	 class HWLayer>
class SPIInterface : public Communication< HWLayer > {
private:
  typedef Communication< HWLayer > CommLayer;

public:
  /* ------------------------------------------------------------------------ */
  static void activate() {
    /* set SS to output and HIGH to deselect everything */
    _ss::output();
    _ss::high();

    /* Set MOSI and SCK output, all others input */
    _mosi::output();
    _sck::output();
  }

  static inline void select() {
    _ss::low();
  }

  static inline void unselect() {
    _ss::high();
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline void send(const T & data) {
    select();
    CommLayer::send(data);
    unselect();
  }

  template<typename T>
  static inline void send(const T data[], uint8_t size) {
    select();
    CommLayer::send(data, size);
    unselect();
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline void receive(T & data) {
    select();
    CommLayer::receive(data);
    unselect();
  }

  template<typename T>
  static inline void receive(T data[], uint8_t size) {
    select();
    CommLayer::receive(data, size);
    unselect();
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline T transfer(const T & sdata) {
    select();
    T rdata = CommLayer::transfer(sdata);;
    unselect();
    return rdata;
  }

  template<typename T>
  static inline void transfer(const T sdata[], T rdata[], uint8_t ssize, uint8_t rsize = 0) {
    select();
    CommLayer::transfer(sdata, rdata, ssize, rsize);
    unselect();
  }

  /* ------------------------------------------------------------------------ */
  static void print(const char * str = "") {
    select();
    CommLayer::print(str);
    unselect();
  }


  /* ------------------------------------------------------------------------ */
  static void println(const char * str = "") {
    select();
    CommLayer::print(str);
    unselect();
  }
};
#endif // __SPI_HH__
