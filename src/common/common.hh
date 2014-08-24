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

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

struct not_defined;

#ifndef __COMMON_HH__
#define __COMMON_HH__

#ifndef cbi
#  define cbi(sfr, bit) (sfr &= ~_BV(bit))
#endif

#ifndef sbi
#  define sbi(sfr, bit) (sfr |= _BV(bit))
#endif

#define OUTPUT  0x1
#define INPUT  0x0

#define HIGH 0x1
#define LOW  0x0

enum type_id_t {
  _type_unused,
  _type_timer_channel,
  _type_timer,
  _type_adc,
  _type_external_interrupt
};


#include "common/unused.hh"


/* -------------------------------------------------------------------------- */
template < typename T1, typename T2 >
struct is_same {
  enum { value = false };      // is_same represents a bool.
};

template < typename T >
struct is_same<T, T> {
  enum { value = true };
};

#define min(x, y) ((x) > (y) ? (y) : (x))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define swap(a, b) { uint8_t __t = (a); (a) = (b); (b) = __t; }

/* -------------------------------------------------------------------------- */
template<typename T1, typename T2, typename T3, uint8_t type_select,
         uint8_t type_id_1 = T1::type_id,
         uint8_t type_id_2 = T2::type_id,
         uint8_t type_id_3 = T3::type_id>
struct select_type {
  typedef unused_type type;
};

template<typename T1, typename T2, typename T3,
         uint8_t type_select, uint8_t type_id_2, uint8_t type_id_3>
struct select_type<T1, T2, T3, type_select, type_select, type_id_2, type_id_3> {
  typedef T1 type;
};

template<typename T1, typename T2, typename T3,
         uint8_t type_select, uint8_t type_id_1, uint8_t type_id_3>
struct select_type<T1, T2, T3, type_select, type_id_1, type_select, type_id_3> {
  typedef T2 type;
};

template<typename T1, typename T2, typename T3,
         uint8_t type_select, uint8_t type_id_1, uint8_t type_id_2>
struct select_type<T1, T2, T3, type_select, type_id_1, type_id_2, type_select> {
  typedef T3 type;
};

#include "conf/mcu_conf.hh"

#endif //__COMMON_HH__
