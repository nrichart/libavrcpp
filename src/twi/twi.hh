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

#ifndef __TWI_HH__
#define __TWI_HH__

/* -------------------------------------------------------------------------- */
#include <util/twi.h>
/* -------------------------------------------------------------------------- */

#if !defined(TWI_RECEIVE_BUFFER_SIZE)
#define TWI_RECEIVE_BUFFER_SIZE 32
#elif TWI_RECEIVE_BUFFER_SIZE > 255
#define TWI_RECEIVE_BUFFER_SIZE 255
#endif

#if !defined(TWI_SEND_BUFFER_SIZE)
#define TWI_SEND_BUFFER_SIZE 32
#elif TWI_SEND_BUFFER_SIZE > 255
#define TWI_SEND_BUFFER_SIZE 255
#endif

namespace twi {

enum _stop_t { _non_stop = 0x00, _stop = 0x01 };

/* -------------------------------------------------------------------------- */
typedef twcr::bit<0> twie;
typedef twcr::bit<2> twen;
typedef twcr::bit<3> twwc;
typedef twcr::bit<4> twsto;
typedef twcr::bit<5> twsta;
typedef twcr::bit<6> twea;
typedef twcr::bit<7> twint;
/* -------------------------------------------------------------------------- */
typedef twsr::bit<0> twps0;
typedef twsr::bit<1> twps1;
typedef twsr::bit<3> tws3;
typedef twsr::bit<4> tws4;
typedef twsr::bit<5> tws5;
typedef twsr::bit<6> tws6;
typedef twsr::bit<7> tws7;
/* -------------------------------------------------------------------------- */
typedef twar::bit<0> twgce;
typedef twar::bit<1> twa0;
typedef twar::bit<2> twa1;
typedef twar::bit<3> twa2;
typedef twar::bit<4> twa3;
typedef twar::bit<5> twa4;
typedef twar::bit<6> twa5;
typedef twar::bit<7> twa6;

/* -------------------------------------------------------------------------- */
namespace clock {
enum _prescaler_t : uint8_t {
  _clk_1 = 0x00,
  _clk_4 = 0x01,
  _clk_16 = 0x02,
  _clk_64 = 0x03
};
} // clock

namespace status {
enum : uint8_t {
  _start = 0x08,
  _rep_start = 0x10,
  _mt_sla_ack = 0x18,
  _mt_sla_nack = 0x20,
  _mt_data_ack = 0x28,
  _mt_data_nack = 0x30,
  _mt_arb_lost = 0x38,
  _mr_arb_lost = 0x38,
  _mr_sla_ack = 0x40,
  _mr_sla_nack = 0x48,
  _mr_data_ack = 0x50,
  _mr_data_nack = 0x58,
  _sr_sla_ack = 0x60,
  _sr_arb_lost_sla_ack = 0x68,
  _sr_gcall_ack = 0x70,
  _sr_arb_lost_gcall_ack = 0x78,
  _sr_data_ack = 0x80,
  _sr_data_nack = 0x88,
  _sr_gcall_data_ack = 0x90,
  _sr_gcall_data_nack = 0x98,
  _sr_stop = 0xA0,
  _st_sla_ack = 0xA8,
  _st_arb_lost_sla_ack = 0xB0,
  _st_data_ack = 0xB8,
  _st_data_nack = 0xC0,
  _st_last_data = 0xC8,
  _no_info = 0xF8,
  _bus_error = 0x00
};
} // status

namespace mask {
enum : uint8_t {
  _status = _or<tws7, tws6, tws5, tws4, tws3>::value,
  _addr = _or<twa0, twa1, twa2, twa3, twa4, twa5, twa6>::value,

  _prescaler = _or<twps0, twps1>::value,

  _activate = _or<twint, twsta, twsto, twea, twen, twie>::value,
  _deactivate = _or<twea, twie>::value,

  _comm = _or<twint, twsta, twsto>::value,
  _comm_ack = _or<twint, twsta, twsto, twea>::value,
  _comm_ack_slave = _or<twint, twsto, twea>::value
};
} // mask

namespace state {
enum state_t {
  _ready = 0x00,
  _write = 0x01,
  _read = 0x02,
  _gcall = 0x04,
  _master = 0x10,
  _slave = 0x20,
  _send_stop = 0x40,
  _repeat_start = 0x80
};
} // state

namespace sla {
enum : uint8_t { _write = 0, _read = 1 };
}

namespace action {
enum : uint8_t {
  _start = _or<twint, twsta>::value,
  _clear_int = twint::value,
  _stop = _or<twint, twsto>::value,
  _activate = _or<twea, twen, twie>::value,
  _nack = twint::value,
  _ack = _or<twint, twea>::value,
};
} // twcr
}

/* -------------------------------------------------------------------------- */
template <typename _sda, typename _scl>
class TWIInterface : public Communication<TWIInterface<_sda, _scl>> {
  typedef Communication<TWIInterface<_sda, _scl>> CommLayer;

public:
  static void activate(uint8_t addr, uint8_t slave_addr, bool _slave = false) {
    slave = _slave;

    // set the slave address
    twar::sbits<twi::twa0::value, twi::mask::_addr>(slave_addr << 1);

    state = twi::state::_ready;
    send_buffer.flush();
    receive_buffer.flush();

    twcr::set(twi::action::_activate);
  }

  static void deactivate(uint8_t addr, bool _slave = false) {
    twcr::sbits<0, twi::mask::_deactivate>(0);
  }

  /* ------------------------------------------------------------------------ */
  static inline void setPrescaler(twi::clock::_prescaler_t prescaler,
                                  uint32_t freq) {
    twsr::sbits<twi::twps0::value, twi::mask::_prescaler>(prescaler);
    twbr::set(((F_CPU / freq) - 16) / (2 * prescaler));
  }

  /* ------------------------------------------------------------------------ */
  static void sendByte(const uint8_t &data) {
    if (!send_buffer.isFull()) {
      send_buffer.push(data);
    }
  }

  /* ------------------------------------------------------------------------ */
  static uint8_t receiveByte() {
    if (!receive_buffer.isEmpty()) {
      return receive_buffer.pop();
    } else {
      return 0;
    }
  }

  static uint8_t available() { return nb_recieved; }

private:
  static void startTransmission(uint8_t rw, uint8_t _addr, twi::_stop_t s) {
    state |= twi::state::_master | rw | (s << 6);

    if (state & twi::state::_repeat_start) {
      state &= ~twi::state::_repeat_start; // clear the repeat restart status
      if (addr == _addr) {
        twdr::set(
            (rw & twi::state::_write ? twi::sla::_write : twi::sla::_read) |
            (_addr << 1));
        ack();
      } else {
        addr = _addr;
        stop(twi::_non_stop);
        state |= twi::state::_master | rw | (s << 6);
      }
    } else {
      addr = _addr;
      start();
    }
  }

protected:
  static uint8_t _receiveFrom(uint8_t _addr, uint8_t size, twi::_stop_t s) {
    // wait nothing else is going on at the same time
    waitReady();
    nb_recieved = 0;
    nb_to_recieve = size - 1;

    startTransmission(twi::state::_read, _addr, s);
    waitReady();
    return nb_recieved;
  }

  static void _sendTo(uint8_t _addr, twi::_stop_t s) {
    // wait nothing else is going on at the same time
    waitReady();
    nb_send = send_buffer.used();
    startTransmission(twi::state::_write, _addr, s);
  }

private:
  static void callbackRead() {}

  static void callbackWrite() {}

private:
  static void store_received() {
    receive_buffer.push(twdr::val());
    if (!slave)
      --nb_to_recieve;
    ++nb_recieved;
  }

  static void send_stored() {
    uint8_t data = send_buffer.pop();
    twdr::set(data);
    --nb_send;
  }

  static void sla(uint8_t rw) { twdr::set((addr << 1) | rw); }

  static inline void waitReady() {
    do {
      _delay_ms(1);
    } while ((state & 0x7F) != twi::state::_ready);
  }

  static void ack(bool full = true) {
    if (full)
      twcr::sbits<0, twi::mask::_comm_ack>(twi::action::_ack);
    else
      twcr::sbits<0, twi::mask::_comm_ack_slave>(twi::action::_ack);
  }

  static void nack(bool full = true) {
    if (full)
      twcr::sbits<0, twi::mask::_comm_ack>(twi::action::_nack);
    else
      twcr::sbits<0, twi::mask::_comm_ack_slave>(twi::action::_nack);
  }

  static void clearInt() {
    twcr::sbits<0, twi::mask::_comm>(twi::action::_clear_int);
  }

  static void stop(twi::_stop_t stop = twi::_stop) {
    if (stop) { // stop
      twcr::sbits<0, twi::mask::_comm>(twi::action::_stop);
    } else { // stop followed by start
      twcr::sbits<0, twi::mask::_comm>(twi::action::_stop |
                                       twi::action::_start);
    }

    twi::twsto::wait_is_clear();
    state = twi::state::_ready;
  }

  static void repeatStart() {
    twcr::sbits<0, twi::mask::_comm>(
        twi::action::_start); // TWINT + TWSTA + ~TWIE
    state = twi::state::_repeat_start;
  }

  static void start() { twcr::sbits<0, twi::mask::_comm>(twi::action::_start); }

  static uint8_t status() { return twsr::val() | twi::mask::_status; }

private:
  friend void TWI_vect(void);

private:
  static uint8_t addr;
  static uint8_t state;
  static uint8_t slave;
  static uint8_t nb_send;
  static uint8_t nb_to_recieve;
  static uint8_t nb_recieved;

  static Queue<uint8_t, uint8_t, TWI_RECEIVE_BUFFER_SIZE> receive_buffer;
  static Queue<uint8_t, uint8_t, TWI_SEND_BUFFER_SIZE> send_buffer;
};

template <typename _sda, typename _scl>
Queue<uint8_t, uint8_t, TWI_RECEIVE_BUFFER_SIZE>
    TWIInterface<_sda, _scl>::receive_buffer;

template <typename _sda, typename _scl>
Queue<uint8_t, uint8_t, TWI_SEND_BUFFER_SIZE>
    TWIInterface<_sda, _scl>::send_buffer;

template <typename _sda, typename _scl> uint8_t TWIInterface<_sda, _scl>::addr;

template <typename _sda, typename _scl> uint8_t TWIInterface<_sda, _scl>::state;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl>::nb_send;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl>::nb_to_recieve;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl>::nb_recieved;

/* -------------------------------------------------------------------------- */
/* TWI                                                                        */
/* -------------------------------------------------------------------------- */
template <uint8_t addr, typename _sda, typename _scl, uint8_t slave_addr = 0x00>
class TWI : public TWIInterface<_sda, _scl> {
public:
  typedef TWIInterface<_sda, _scl> interface;

  /* ------------------------------------------------------------------------ */
  static void
  activate(bool slave, uint32_t freq = 100000L,
           twi::clock::_prescaler_t prescaler = twi::clock::_clk_1) {
    // set pull-up resistors
    _sda::high();
    _scl::high();

    // set the bite rate
    interface::setPrescaler(prescaler, freq);

    interface::activate(addr, slave_addr, slave);
  }

  static void deactivate() {
    _sda::low();
    _scl::low();
    interface::deactivate();
  }

  /* ------------------------------------------------------------------------ */
  template <typename T> static void registerRead(uint8_t reg, T &value) {
    send(reg, twi::_non_stop);
    receive(value, twi::_stop);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static void registerRead(uint8_t reg, T value[], uint8_t size) {
    send(reg, twi::_non_stop);
    receive(value, size, twi::_stop);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T> static void registerWrite(uint8_t reg, const T &value) {
    interface::send(reg);
    interface::send(value);
    sendTo();
  }

  /* ------------------------------------------------------------------------ */
  static inline void sendTo(twi::_stop_t s = twi::_stop) {
    interface::_sendTo(addr, s);
  }

  /* ------------------------------------------------------------------------ */
  static inline void receiveFrom(uint8_t size, twi::_stop_t s = twi::_stop) {
    interface::_receiveFrom(addr, size, s);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static inline void send(const T &data, twi::_stop_t s = twi::_stop) {
    // pack the data
    interface::send(data);
    // send them
    sendTo(s);
  }

  template <typename T>
  static inline void send(const T data[], uint8_t size,
                          twi::_stop_t s = twi::_stop) {
    // pack the data
    interface::send(data, size);
    // send them
    sendTo(s);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static inline void receive(T &data, twi::_stop_t s = twi::_stop) {
    // get the data
    receiveFrom(sizeof(T), s);
    // unpack them
    interface::receive(data);
  }

  template <typename T>
  static inline void receive(T data[], uint8_t size,
                             twi::_stop_t s = twi::_stop) {
    // get the data
    receiveFrom(size * sizeof(T), s);
    // unpack them
    interface::receive(data, size);
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static inline T transfer(const T &sdata, twi::_stop_t s = twi::_stop) {
    send(sdata, twi::_non_stop);
    T rdata;
    receive(rdata, s);
    return rdata;
  }

  /* ------------------------------------------------------------------------ */
  template <typename T>
  static inline void transfer(const T sdata[], T rdata[], uint8_t ssize,
                              uint8_t rsize, twi::_stop_t s = twi::_stop) {
    send(sdata, ssize, twi::_non_stop);
    receive(rdata, rsize, s);
  }
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#if !defined(NO_DEFAULT_TWI)
template<uint8_t addr, uint8_t slave_addr = 0x00>
using TWI0 = TWI<addr, scl, sda, slave_addr>;
#endif
/* -------------------------------------------------------------------------- */

#endif //__TWI_HH__
