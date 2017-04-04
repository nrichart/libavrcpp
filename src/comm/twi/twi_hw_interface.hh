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
#include "twi/twi_interface.hh"
/* -------------------------------------------------------------------------- */

#ifndef TWI_HW_INTERFACE_HH
#define TWI_HW_INTERFACE_HH

namespace twi {

template <typename _sda, typename _scl>
class TWIInterface<_sda, _scl, _hw_twi> :
    public Communication<TWIInterface<_sda, _scl, _hw_twi>> {
  typedef Communication<TWIInterface<_sda, _scl, _hw_twi>> CommLayer;

public:
  static void activate(uint8_t addr, uint8_t slave_addr, bool _slave = false) {
    // set pull-up resistors
    _sda::high();
    _scl::high();

    slave = _slave;

    // set the slave address
    twi::twa::set(slave_addr);

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
    twi::twps::set(prescaler);
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
    twcr::sbits<0, mask::_comm>(twi::action::_start); // TWINT + TWSTA + ~TWIE
    state = twi::state::_repeat_start;
  }

  static void start() { twcr::sbits<0, mask::_comm>(action::_start); }

  static status::_status_t status() { return status::_status_t(tws::get()); }

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
    TWIInterface<_sda, _scl, _hw_twi>::receive_buffer;

template <typename _sda, typename _scl>
Queue<uint8_t, uint8_t, TWI_SEND_BUFFER_SIZE>
    TWIInterface<_sda, _scl, _hw_twi>::send_buffer;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl, _hw_twi>::addr;

template <typename _sda, typename _scl>
uint8_t TWIInterface<_sda, _scl, _hw_twi>::state;

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
  using namespace status;
  switch (status()) {
  case _start:
  case _rep_start: {
    if (state & state::_write)
      sla(sla::_write);
    else
      sla(sla::_read);

    clearInt();
    break;
  }
  /* -- Master transmit ------------------------------------------------- */
  case _mt_sla_ack:   // slave acked address
  case _mt_data_ack: {// slave acked data
    // if still data to send, send the next byte
    if (nb_send) {
      send_stored();
      clearInt();
    } else {
      // else stop or repeat start
      if (state & state::_send_stop)
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
    if (state & state::_send_stop)
      stop();
    else
      repeatStart();
    break;
  case _sr_gcall_ack:
  case _sr_arb_lost_gcall_ack:
    state |= state::_gcall;
  case _sr_sla_ack:
  case _sr_arb_lost_sla_ack:
    state |= state::_slave | state::_read;
    ack();
    break;
  case _sr_data_ack:
  case _sr_gcall_data_ack:
    if(receive_buffer.isFull()) {
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
    state = state::_ready;
    break;
  case _st_sla_ack:
  case _st_arb_lost_sla_ack:
    state |= state::_slave | state::_write;
    callbackWrite();
  case _st_data_ack:
    send_stored();
    if (nb_send) ack(false);
    else nack(false);
    break;
  case _st_data_nack:
  case _st_last_data:
    ack();
    state = state::_ready;
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
