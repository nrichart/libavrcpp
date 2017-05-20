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
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
/* -------------------------------------------------------------------------- */

struct not_defined;

#ifndef COMMON_HH
#define COMMON_HH

enum : uint8_t { OUTPUT = 0x1, INPUT = 0x0 };
enum : uint8_t { HIGH = 0x1, LOW = 0x0 };

enum type_id_t {
  _type_unused,
  _type_timer_channel,
  _type_timer,
  _type_adc,
  _type_external_interrupt
};

#include "common/unused.hh"
/* -------------------------------------------------------------------------- */
#include "common/type_traits.hh"
/* -------------------------------------------------------------------------- */
template <uint8_t type_id_t, typename T, typename = void>
struct _select_type_helper {
  using type = unused_type;
};

template <uint8_t type_id_t, typename T>
struct _select_type_helper<type_id_t, T,
                           std::enable_if_t<T::type_id == type_id_t, T>> {
  using type = T;
};

template <uint8_t type_id_t, typename... T> struct select_type_helper {
  using type = unused_type;
};

template <uint8_t type_id_t, typename T, typename... Ts>
struct select_type_helper<type_id_t, T, Ts...> {
  using _type1 = typename _select_type_helper<type_id_t, T>::type;
  using _type2 = typename select_type_helper<type_id_t, Ts...>::type;
  using type =
      std::conditional_t<std::is_same<T, _type1>::value, _type1, _type2>;
};

template <uint8_t type_id_t, typename... Ts> struct select_type {
  using type = typename select_type_helper<type_id_t, Ts...>::type;
};

template <uint8_t type_id_t, typename... Ts>
using select_type_t = typename select_type<type_id_t, Ts...>::type;

/* -------------------------------------------------------------------------- */
#include "conf/mcu_conf.hh"
/* -------------------------------------------------------------------------- */

#endif // COMMON_HH
