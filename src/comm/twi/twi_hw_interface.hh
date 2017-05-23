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

#ifndef TWI_HW_INTERFACE_HH
#define TWI_HW_INTERFACE_HH

/* -------------------------------------------------------------------------- */
#include "common/common.hh"
#include "common/communication.hh"
#include "common/queue.hh"
#include "twi/twi_interface.hh"
/* -------------------------------------------------------------------------- */

namespace twi {

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

enum status_t : uint8_t {
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

/* -------------------------------------------------------------------------- */
template <typename _sda, typename _scl>
class TWIInterface<_sda, _scl, _hw_twi>
  : public Communication<TWIInterface<_sda, _scl, _hw_twi>> {
private:
  typedef TWIInterface<_sda, _scl, _hw_twi> my_type;
  typedef Communication<my_type> CommLayer;

private:
  static void ext_activate();

  typedef twcr::bit<0> twie;
  typedef twcr::bit<2> twen;
  typedef twcr::bit<3> twwc;
  typedef twcr::bit<4> twsto;
  typedef twcr::bit<5> twsta;
  typedef twcr::bit<6> twea;
  typedef twcr::bit<7> twint;
  /* --------------------------------------------------------------------------
   */
  typedef twsr::bit<0> twps0;
  typedef twsr::bit<1> twps1;
  typedef twsr::bits<2, 0x3> twps;
  typedef twsr::bit<3> tws3;
  typedef twsr::bit<4> tws4;
  typedef twsr::bit<5> tws5;
  typedef twsr::bit<6> tws6;
  typedef twsr::bit<7> tws7;
  typedef twsr::bits<2, 0x1F> tws;
  /* --------------------------------------------------------------------------
   */
  typedef twar::bit<0> twgce;
  typedef twar::bit<1> twa0;
  typedef twar::bit<2> twa1;
  typedef twar::bit<3> twa2;
  typedef twar::bit<4> twa3;
  typedef twar::bit<5> twa4;
  typedef twar::bit<6> twa5;
  typedef twar::bit<7> twa6;
  typedef twar::bits<1, 0x7F> twa;

  enum mask_t : uint8_t {
    //    _activate = _or<twint, twsta, twsto, twea, twen, twie>::value,
    _deactivate = _or<twea, twie>::value,

    _comm = _or<twint, twsta, twsto>::value,
    _comm_ack = _or<twint, twsta, twsto, twea>::value,
    _comm_ack_slave = _or<twint, twsto, twea>::value
  };

  enum action_t : uint8_t {
    _start_condition = _or<twint, twsta>::value,
    _clear_int = twint::value,
    _stop_condition = _or<twint, twsto>::value,
    _activate = _or<twea, twen, twie>::value,
    _nack = twint::value,
    _ack = _or<twint, twea>::value,
  };

public:
  static void activate(uint8_t addr, uint8_t slave_addr, bool _slave = false) {

    // set pull-up resistors
    _sda::high();
    _scl::high();

    slave = _slave;

    // set the slave address
    twa::set(slave_addr);

    state = _ready;
    send_buffer.flush();
    receive_buffer.flush();

    twcr::set(_activate);

    ext_activate();
  }

  static void deactivate(uint8_t addr, bool _slave = false) {
    twcr::sbits<0, _deactivate>(0);
  }

  /* ------------------------------------------------------------------------ */
  static inline void setPrescaler(clock::_prescaler_t prescaler,
                                  uint32_t freq) {
    twps::set(prescaler);
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
  static void startTransmission(uint8_t rw, uint8_t _addr, stop_t s) {
    state |= _master | rw | (s << 6);

    if (state & _repeat_start) {
      state &= ~_repeat_start; // clear the repeat restart status
      if (addr == _addr) {
        twdr::set((rw & _write ? sla::_write : sla::_read) | (_addr << 1));
        ack();
      } else {
        addr = _addr;
        stop(_non_stop);
        state |= _master | rw | (s << 6);
      }
    } else {
      addr = _addr;
      start();
    }
  }

protected:
  static uint8_t receiveFrom(uint8_t _addr, uint8_t size, stop_t s) {
    // wait nothing else is going on at the same time
    waitReady();
    nb_recieved = 0;
    nb_to_recieve = size - 1;

    startTransmission(_read, _addr, s);
    waitReady();
    return nb_recieved;
  }

  static void sendTo(uint8_t _addr, stop_t s) {
    // wait nothing else is going on at the same time
    waitReady();
    nb_send = send_buffer.used();
    startTransmission(_write, _addr, s);
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
    } while ((state & 0x7F) != _ready);
  }

  static void ack(bool full = true) {
    if (full)
      twcr::sbits<0, _comm_ack>(_ack);
    else
      twcr::sbits<0, _comm_ack_slave>(_ack);
  }

  static void nack(bool full = true) {
    if (full)
      twcr::sbits<0, _comm_ack>(_nack);
    else
      twcr::sbits<0, _comm_ack_slave>(_nack);
  }

  static void clearInt() { twcr::sbits<0, _comm>(_clear_int); }

  static void stop(stop_t stop = _stop) {
    if (stop) { // stop
      twcr::sbits<0, _comm>(_stop_condition);
    } else { // stop followed by start
      twcr::sbits<0, _comm>(_stop_condition | _start_condition);
    }

    twsto::wait_is_clear();
    state = _ready;
  }

  static void repeatStart() {
    twcr::sbits<0, _comm>(_start_condition); // TWINT + TWSTA + ~TWIE
    state = _repeat_start;
  }

  static void start() { twcr::sbits<0, _comm>(_start_condition); }

  static status_t status() { return status_t(tws::get()); }

public:
  static inline void TWI_vect();

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
TWIInterface<_sda, _scl,  _hw_twi>::receive_buffer;

template <typename _sda, typename _scl>
Queue<uint8_t, uint8_t, TWI_SEND_BUFFER_SIZE>
TWIInterface<_sda, _scl, _hw_twi>::send_buffer;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl, _hw_twi>::addr;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl, _hw_twi>::state;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl, _hw_twi>::slave;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl, _hw_twi>::nb_send;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl, _hw_twi>::nb_to_recieve;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl, _hw_twi>::nb_recieved;

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
template <typename _sda, typename _scl>
void TWIInterface<_sda, _scl, _hw_twi>::TWI_vect() {
  switch (status()) {
  case _start:
  case _rep_start: {
    if (state & _write)
      sla(sla::_write);
    else
      sla(sla::_read);

    clearInt();
    break;
  }
  /* -- Master transmit ------------------------------------------------- */
  case _mt_sla_ack:    // slave acked address
  case _mt_data_ack: { // slave acked data
    // if still data to send, send the next byte
    if (nb_send) {
      send_stored();
      clearInt();
    } else {
      // else stop or repeat start
      if (state & _send_stop)
        stop(_stop);
      else
        repeatStart();
    }
    break;
  }
  case _mt_sla_nack:
  case _mt_data_nack:
    // should probably handle the error;
    stop(_stop);
    break;
  case _mt_arb_lost:
    // lost arbitration, send a start to try again once the bus is free
    start();
    break;
  case _mr_data_ack:
    store_received();
  case _mr_sla_ack:
    if (nb_to_recieve == 0) {
      nack();
    } else {
      ack();
    }
    break;
  // -- Master receive -------------------------------------------------- */
  case _mr_sla_nack:
    // should probably handle the error;
    stop(_stop);
    break;
  case _mr_data_nack:
    store_received();
    if (state & _send_stop)
      stop();
    else
      repeatStart();
    break;
  case _sr_gcall_ack:
  case _sr_arb_lost_gcall_ack:
    state |= _gcall;
  case _sr_sla_ack:
  case _sr_arb_lost_sla_ack:
    state |= _slave | _read;
    ack();
    break;
  case _sr_data_ack:
  case _sr_gcall_data_ack:
    if (receive_buffer.isFull()) {
      nack(false);
    } else {
      store_received();
      ack(false);
    }
    break;
  case _sr_data_nack:
  case _sr_gcall_data_nack:
    store_received();
    nack();
    break;
  case _sr_stop:
    ack();

    callbackRead();
    state = _ready;
    break;
  case _st_sla_ack:
  case _st_arb_lost_sla_ack:
    state |= _slave | _write;
    callbackWrite();
  case _st_data_ack:
    send_stored();
    if (nb_send)
      ack(false);
    else
      nack(false);
    break;
  case _st_data_nack:
  case _st_last_data:
    ack();
    state = _ready;
    break;
  case _no_info:
    break;
  case _bus_error:
    stop();
    break;
  }
}
/* -------------------------------------------------------------------------- */

} // namespace twi

#endif /* TWI_HW_INTERFACE_HH */
