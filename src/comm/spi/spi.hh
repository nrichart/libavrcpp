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
#include "spi/spi_interface.hh"
/* -------------------------------------------------------------------------- */

#ifndef SPI_HH
#define SPI_HH

/* -------------------------------------------------------------------------- */
#if !defined(SPI_INTERFACE)
#  define SPI_INTERFACE spi::_hw_spi
#endif
/* -------------------------------------------------------------------------- */

template<typename _sck, typename _miso, typename _mosi, typename _ss,
         spi::spi_interface_t _interface = spi::_hw_spi>
class SPI : public spi::SPIInterface<_sck, _miso, _mosi, _ss, _interface> {
private:
  typedef spi::SPIInterface<_sck, _miso, _mosi, _ss, _interface> interface;

public:
  /* ------------------------------------------------------------------------ */
  static void activate() {
    /* set SS to output and HIGH to deselect everything */
    _ss::output();
    _ss::high();

    /* Set MOSI and SCK output, all others input */
    _mosi::output();
    _sck::output();

    interface::activate();
  }

  static inline void select() {
    interface::select();
    _ss::low();
  }

  static inline void unselect() {
    _ss::high();
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline void send(const T & data) {
    select();
    interface::send(data);
    unselect();
  }

  template<typename T>
  static inline void send(const T data[], uint8_t size) {
    select();
    interface::send(data, size);
    unselect();
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline void receive(T & data) {
    select();
    interface::receive(data);
    unselect();
  }

  template<typename T>
  static inline void receive(T data[], uint8_t size) {
    select();
    interface::receive(data, size);
    unselect();
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline T transfer(const T & sdata) {
    select();
    T rdata = interface::transfer(sdata);;
    unselect();
    return rdata;
  }

  template<typename T>
  static inline void transfer(const T sdata[], T rdata[], uint8_t ssize, uint8_t rsize = 0) {
    select();
    interface::transfer(sdata, rdata, ssize, rsize);
    unselect();
  }

  /* ------------------------------------------------------------------------ */
  static void print(const char * str = "") {
    select();
    interface::print(str);
    unselect();
  }


  /* ------------------------------------------------------------------------ */
  static void println(const char * str = "") {
    select();
    interface::print(str);
    unselect();
  }
};

/* -------------------------------------------------------------------------- */
#if !defined(NO_DEFAULT_SPI)
template <typename _ss = ss>
using SPI0 = SPI<sck, mosi, mosi, _ss, SPI_INTERFACE>;
#endif

#endif // __SPI_HH__
