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
#include <avr/sfr_defs.h>
#include <stdint.h>
/* -------------------------------------------------------------------------- */

#ifndef REGISTERS_HH
#define REGISTERS_HH

/* -------------------------------------------------------------------------- */
template <typename _reg_t> struct reg_high {
  enum { offset = 1 };
};
template <> struct reg_high<uint8_t> {
  enum { offset = 0 };
};

/* -------------------------------------------------------------------------- */
template <typename reg, typename _reg_t> struct regReaderHelper {
  static inline _reg_t val() { return reg::val_low(); }
};

template <typename reg> struct regReaderHelper<reg, uint16_t> {
  static inline uint16_t val() {
    uint16_t v = reg::low::val_low() | (reg::high::val_low() << 8);
    return v;
  }
};

/* -------------------------------------------------------------------------- */
template <typename reg, typename _reg_t> struct regWriterHelper {
  static inline void set(_reg_t val) {
    (*((volatile _reg_t *)reg::addr)) = val;
  }
};

template <typename reg> struct regWriterHelper<reg, uint16_t> {
  static inline void set(uint16_t val) {
    reg::high::set(val >> 8);
    reg::low::set(val & 0xFF);
  }
};

/* -------------------------------------------------------------------------- */
template <typename reg, uint8_t b> class bit;
template <typename reg, uint8_t b, uint8_t mask> class bits;
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Registers                                                                  */
/* -------------------------------------------------------------------------- */
template <uint8_t address, typename _reg_t = uint8_t> class reg {
public:
  template <uint8_t b> using bit = bit<reg<address, _reg_t>, b>;
  template <uint8_t b, uint8_t mask> using bits = bits<reg<address, _reg_t>, b, mask>;

  enum { addr = address };
  typedef reg<address, uint8_t> low;
  typedef reg<address + reg_high<_reg_t>::offset, uint8_t> high;
  typedef _reg_t type;

  static inline volatile _reg_t *ptr() { return ((volatile _reg_t *)address); }

  static inline void cbit(uint8_t bit) {
    *((volatile uint8_t *)address) &= ~_BV(bit);
  }

  static inline void sbit(uint8_t bit) {
    *((volatile _reg_t *)address) |= _BV(bit);
  }

  static inline uint8_t getbit(uint8_t bit) {
    return *((volatile _reg_t *)address) & _BV(bit);
  }

  static inline bool is_clear(uint8_t bit) { return getbit(bit) == 0; }
  static inline bool is_set(uint8_t bit) { return getbit(bit) == 1; }

  static inline void wait_bit_is_clear(uint8_t bit) {
    do {
    } while (getbit(bit));
  }

  static inline void wait_bit_is_set(uint8_t bit) {
    do {
    } while (!getbit(bit));
  }

  static inline _reg_t val_low() { return (*((volatile _reg_t *)address)); }

  static inline _reg_t val() {
    return regReaderHelper<reg<address, _reg_t>, _reg_t>::val();
  }

  static inline void set(_reg_t val) {
    regWriterHelper<reg<address, _reg_t>, _reg_t>::set(val);
  }

  template <uint8_t pos, uint8_t mask> static inline void sbits(_reg_t v) {
    set((val() & (0xFF - (mask << pos))) | ((v & mask) << pos));
  }

  static inline void andequal(uint8_t val) {
    (*((volatile _reg_t *)address)) &= val;
  }
};

/* -------------------------------------------------------------------------- */
template <uint8_t address, typename _reg_t = uint8_t>
class reg_io : public reg<address + __SFR_OFFSET, _reg_t> {};

/* -------------------------------------------------------------------------- */
/* Bit                                                                        */
/* -------------------------------------------------------------------------- */
template <typename reg, uint8_t b> class bit {
public:
  enum : uint8_t { value = (1 << b) };
  static inline void clear() { reg::cbit(b); }
  static inline void set() { reg::sbit(b); }
  static inline uint8_t get() { return reg::getbit(b); }
  static inline void wait_is_clear() { reg::wait_bit_is_clear(b); }
  static inline void wait_is_set() { reg::wait_bit_is_set(b); }
};

/* -------------------------------------------------------------------------- */
template <class bit> class _tilde {
public:
  enum : uint8_t { value = ~bit::value };
};

template <typename... bits> class _or {};

template <typename bit1, typename bit2> class _or<bit1, bit2> {
public:
  enum : uint8_t { value = bit1::value | bit2::value };
};

template <typename bit, typename... bits> class _or<bit, bits...> {
public:
  enum : uint8_t { value = bit::value | _or<bits...>::value };
};

template <typename... bits> class _and {};

template <typename bit1, typename bit2> class _and<bit1, bit2> {
public:
  enum : uint8_t { value = bit1::value & bit2::value };
};

template <typename bit, typename... bits> class _and<bit, bits...> {
public:
  enum : uint8_t { value = bit::value & _and<bits...>::value };
};

/* -------------------------------------------------------------------------- */
/* Bits                                                                       */
/* -------------------------------------------------------------------------- */
template <typename reg, uint8_t start_bit, uint8_t mask> class bits {
public:
  static inline void set(typename reg::type val) {
    reg::sbits<start_bit, mask>(val);
  }
  static inline typename reg::type get() {
    return ((reg::val() >> start_bit) & mask);
  }
};



#endif /* __REGISTERS_HH__ */
