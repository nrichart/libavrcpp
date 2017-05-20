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

/**
   This is taken from the possible implementation proposed on cpp.reference.org
*/

namespace std {

/* -------------------------------------------------------------------------- */
/* type_traits                                                                */
/* -------------------------------------------------------------------------- */
template <class T, T v> struct integral_constant {
  static constexpr T value = v;
  typedef T value_type;
  typedef integral_constant type; // using injected-class-name
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept {
    return value;
  } // since c++14
};

template <bool B> using bool_constant = integral_constant<bool, B>;

using false_type = bool_constant<false>;
using true_type = bool_constant<true>;

template <typename... Ts> using void_t = void;

template <class T, class U> struct is_same : std::false_type {};

template <class T> struct is_same<T, T> : std::true_type {};

template <bool B, class T = void> struct enable_if {};

template <class T> struct enable_if<true, T> { typedef T type; };

template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <bool B, class T, class F> struct conditional { typedef T type; };

template <class T, class F> struct conditional<false, T, F> { typedef F type; };

template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

/* -------------------------------------------------------------------------- */
/* algorithm                                                                  */
/* -------------------------------------------------------------------------- */
template <class T1, class T2> const auto min(const T1 &x, const T2 &y) {
  return x > y ? y : x;
}

template <class T1, class T2> const auto max(const T1 &x, const T2 &y) {
  return x > y ? x : y;
}

template <class T> void swap(T &a, T &b) {
  T __t = a;
  a = b;
  b = __t;
}

/* -------------------------------------------------------------------------- */
}
/* -------------------------------------------------------------------------- */
