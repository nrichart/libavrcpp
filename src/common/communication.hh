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

#ifndef __COMMUNICTION_HH__
#define __COMMUNICTION_HH__

#include <stdio.h>

extern void debug(uint8_t size);

template<typename HWLayer>
class Communication {
public:
  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline void send(const T & data) {
    for(uint8_t i = 0; i < sizeof(T); ++i) {
      uint8_t byte = (data >> i) & 0xFF;
      HWLayer::sendByte(byte);
    }
  }

  template<typename T>
  static inline void send(const T data[], uint8_t size) {
    for(uint8_t i = 0; i < size; ++i) {
      HWLayer::sendByte(data[i]);
    }
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline void receive(T & data) {
    uint8_t nb_bytes_to_receive = sizeof(data);
    while(nb_bytes_to_receive) {
      uint8_t byte = HWLayer::receiveByte();
      nb_bytes_to_receive--;
      data = (data << 8) | byte;
    }
  }

  template<typename T>
  static inline void receive(T data[], uint8_t size) {
    for(uint8_t i = 0; i < size; ++i) {
      data[i] = HWLayer::receiveByte();
    }
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline T transfer(const T & sdata) {
    T rdata = T();
    uint8_t nb_bytes_to_transfer = sizeof(T);
    while(nb_bytes_to_transfer) {
      nb_bytes_to_transfer--;
      uint8_t sbyte = (sdata >> nb_bytes_to_transfer) & 0xFF;
      uint8_t rbyte = HWLayer::transferByte(sbyte);
      rdata = (rdata << 8) | rbyte;
    }
    return rdata;
  }

  template<typename T>
  static inline void transfer(const T sdata[], T rdata[], uint8_t ssize, uint8_t rsize = 0) {
    uint8_t size = ssize;
    for(uint8_t i = 0; i < size; ++i) {
      rdata[i] = HWLayer::transferByte(sdata[i]);
    }
  }

  /* ------------------------------------------------------------------------ */
  static void printf(const char * format, ...) {
    va_list va_arg_lst;
#define MAX_STR_SIZE 64
    char str[MAX_STR_SIZE];

    va_start(va_arg_lst, format);
    vsnprintf(str, MAX_STR_SIZE, format, va_arg_lst);
    va_end(va_arg_lst);

    print(str);
  }

  static void print(const char * str = "") {
    char * strt = (char *)str;
    while(*strt)
      HWLayer::sendByte(*strt++);
  }


  /* ------------------------------------------------------------------------ */
  static void println(const char * str = "") {
    print(str);
    HWLayer::sendByte('\r');
    HWLayer::sendByte('\n');
  }

  /* ------------------------------------------------------------------------ */
  static uint8_t available() {
    return HWLayer::available();
  }
};

#endif /* __COMMUNICTION_HH__ */
