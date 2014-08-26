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

#include <compat/twi.h>
#include "common/common.hh"


#if !defined(TWI_RECEIVE_BUFFER_SIZE)
#  define TWI_RECEIVE_BUFFER_SIZE 32
#elif TWI_RECEIVE_BUFFER_SIZE > 255
#  define TWI_RECEIVE_BUFFER_SIZE 255
#endif

#if !defined(TWI_SEND_BUFFER_SIZE)
#  define TWI_SEND_BUFFER_SIZE 32
#elif TWI_SEND_BUFFER_SIZE > 255
#  define TWI_SEND_BUFFER_SIZE 255
#endif

enum twi_prescaler_t {
  twi_clk_1   = 0x00,
  twi_clk_4   = 0x01,
  twi_clk_16  = 0x02,
  twi_clk_64  = 0x03
};

enum twi_stop_t {
  twi_non_stop = 0x00,
  twi_stop     = 0x01
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
template<typename _sda, typename _scl, bool slave = false>
class TWIInterface : public Communication< TWIInterface<_sda, _scl, slave> > {
  typedef Communication< TWIInterface<_sda, _scl, slave> > CommLayer;
private:
  enum state_t {
    _ready        = 0x00,
    _write        = 0x01,
    _read         = 0x02,
    _master       = 0x04,
    _slave        = 0x08,
    _send_stop    = 0x40,
    _repeat_start = 0x80
  };

public:
  static void activate(uint8_t addr) {
    // set the slave address
    if(slave) twar::sbits<TWA0, 0xFE>(addr << 1);

    state = _ready;
    send_buffer.flush();
    receive_buffer.flush();

    twcr::set(_BV(TWEA) | _BV(TWEN) | _BV(TWIE));
  }


  /* ------------------------------------------------------------------------ */
  static inline void setPrescaler(twi_prescaler_t prescaler,
				  uint32_t freq) {
    twsr::sbits<TWPS0, 0x03>(prescaler);
    twbr::set(((F_CPU/freq) - 16) / (2*prescaler));
  }

   /* ------------------------------------------------------------------------ */
  static void sendByte(const uint8_t & data) {
    if(!send_buffer.isFull()) {
      send_buffer.push(data);
    }
  }

  /* ------------------------------------------------------------------------ */
  static uint8_t receiveByte() {
    if(!receive_buffer.isEmpty()) {
      return receive_buffer.pop();
    } else {
      return 0;
    }
  }

private:
  static void startTransmission(uint8_t rw, uint8_t _addr, twi_stop_t s) {
    state |= _master | rw | (s << 6);

    if(state & _repeat_start) {
      state &= ~_repeat_start; // clear the repeat restart status
      if(addr == _addr) {
	twdr::set((rw & _write ? TW_WRITE : TW_READ) | (_addr << 1));
	twcr::sbits<0, 0xB5>(0x85); // ~TWSTA + TWEN + TWINT + ~TWSTO + TWIE
      } else {
	addr = _addr;
	stop(twi_non_stop);
	state |= _master | rw | (s << 6);
      }
    } else {
      addr = _addr;
      start();
    }
  }

protected:
  static uint8_t _receiveFrom(uint8_t _addr, uint8_t size, twi_stop_t s) {
    // wait nothing else is going on at the same time
    waitReady();
    nb_recieved = 0;
    nb_to_recieve = size - 1;

    startTransmission(_read, _addr, s);

    waitReady();
    return nb_recieved;
  }

  static void _sendTo(uint8_t _addr, twi_stop_t s) {
    // wait nothing else is going on at the same time
    waitReady();
    nb_send = send_buffer.used();

    startTransmission(_write, _addr, s);
  }

private:
  static inline void waitReady() {
    do { _delay_ms(1); } while((state & 0x7F) != _ready);
  }

  static void ack() {
    twcr::sbits<0, 0xF5>(0xC5);     // TWEA + TWINT + ~TWSTO + ~TWSTA + TWEN;
  }

  static void nack() {
    twcr::sbits<0, 0xF5>(0x85);     // ~TWEA + TWINT + ~TWSTO + ~TWSTA + TWEN;
  }

  static void clearInt() {
    twcr::sbits<0, 0xB5>(0x85);     // ~TWSTA + TWEN + TWINT + ~TWSTO
  }

  static void stop(twi_stop_t stop = twi_stop) {
    if (stop) {
      // stop
      twcr::sbits<0, 0xB5>(0x95);   // TWSTO + TWEN + TWINT + ~TWSTA
    } else {
      // stop followed by start
      twcr::sbits<0, 0xB5>(0xB5);   // TWSTO + TWEN + TWINT + TWSTA
    }

    twcr::wait_bit_is_clear(TWSTO);
    state = _ready;
  }

  static void repeatStart() {
    twcr::sbits<0, 0xB5>(0xA4);   // TWINT + TWSTA + TWSTO + ~TWIE
    state = _repeat_start;
  }

  static void start() {
    twcr::sbits<0, 0xB5>(0xA5);     // TWSTA + TWEA + TWINT + ~TWSTO
  }

  static void store_received() {
    receive_buffer.push(twdr::val());
    --nb_to_recieve;
    ++nb_recieved;
  }

private:
  enum status_t {
    _start                 = TW_START		     ,
    _rep_start		   = TW_REP_START	     ,
    _mt_sla_ack		   = TW_MT_SLA_ACK	     ,
    _mt_sla_nack	   = TW_MT_SLA_NACK	     ,
    _mt_data_ack	   = TW_MT_DATA_ACK	     ,
    _mt_data_nack	   = TW_MT_DATA_NACK	     ,
    _mt_arb_lost	   = TW_MT_ARB_LOST	     ,
    //    _mr_arb_lost	   = TW_MR_ARB_LOST	     ,
    _mr_sla_ack		   = TW_MR_SLA_ACK	     ,
    _mr_sla_nack	   = TW_MR_SLA_NACK	     ,
    _mr_data_ack	   = TW_MR_DATA_ACK	     ,
    _mr_data_nack	   = TW_MR_DATA_NACK	     ,
    _st_sla_ack		   = TW_ST_SLA_ACK	     ,
    _st_arb_lost_sla_ack   = TW_ST_ARB_LOST_SLA_ACK  ,
    _st_data_ack	   = TW_ST_DATA_ACK	     ,
    _st_data_nack	   = TW_ST_DATA_NACK	     ,
    _st_last_data	   = TW_ST_LAST_DATA	     ,
    _sr_sla_ack		   = TW_SR_SLA_ACK	     ,
    _sr_arb_lost_sla_ack   = TW_SR_ARB_LOST_SLA_ACK  ,
    _sr_gcall_ack	   = TW_SR_GCALL_ACK	     ,
    _sr_arb_lost_gcall_ack = TW_SR_ARB_LOST_GCALL_ACK,
    _sr_data_ack	   = TW_SR_DATA_ACK          ,
    _sr_data_nack	   = TW_SR_DATA_NACK	     ,
    _sr_gcall_data_ack	   = TW_SR_GCALL_DATA_ACK    ,
    _sr_gcall_data_nack	   = TW_SR_GCALL_DATA_NACK   ,
    _sr_stop		   = TW_SR_STOP		     ,
    _no_info		   = TW_NO_INFO		     ,
    _bus_error		   = TW_BUS_ERROR
  };

  friend void TWI_vect();

private:
  static uint8_t addr;
  static uint8_t state;
  static uint8_t nb_send;
  static uint8_t nb_to_recieve;
  static uint8_t nb_recieved;

  static Queue<uint8_t, uint8_t, TWI_RECEIVE_BUFFER_SIZE> receive_buffer;
  static Queue<uint8_t, uint8_t, TWI_SEND_BUFFER_SIZE   > send_buffer;
};


template<typename _sda, typename _scl, bool slave>
Queue<uint8_t, uint8_t, TWI_RECEIVE_BUFFER_SIZE> TWIInterface<_sda, _scl, slave>::receive_buffer;

template<typename _sda, typename _scl, bool slave>
Queue<uint8_t, uint8_t, TWI_SEND_BUFFER_SIZE> TWIInterface<_sda, _scl, slave>::send_buffer;

template<typename _sda, typename _scl, bool slave>
uint8_t TWIInterface<_sda, _scl, slave>::addr;

template<typename _sda, typename _scl, bool slave>
uint8_t TWIInterface<_sda, _scl, slave>::state;

template<typename _sda, typename _scl, bool slave>
uint8_t TWIInterface<_sda, _scl, slave>::nb_send;

template<typename _sda, typename _scl, bool slave>
uint8_t TWIInterface<_sda, _scl, slave>::nb_to_recieve;

template<typename _sda, typename _scl, bool slave>
uint8_t TWIInterface<_sda, _scl, slave>::nb_recieved;




/* -------------------------------------------------------------------------- */
/* TWI                                                                        */
/* -------------------------------------------------------------------------- */
template<uint8_t addr = 0x0,
	 typename _sda = sda, typename _scl = scl,
	 bool slave = false>
class TWI : public TWIInterface<_sda, _scl, slave> {
public:
  typedef TWIInterface<_sda, _scl, slave> interface;

  /* ------------------------------------------------------------------------ */
  static void activate(uint32_t freq = 100000L,
		       twi_prescaler_t prescaler = twi_clk_1) {
    // set pull-up resistors
    _sda::high();
    _scl::high();

    // set the bite rate
    interface::setPrescaler(prescaler, freq);

    interface::activate(addr);
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static void registerRead(uint8_t reg, T & value) {
    send(reg, twi_non_stop);
    receive(value, twi_stop);
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static void registerRead(uint8_t reg, T value[], uint8_t size) {
    send(reg, twi_non_stop);
    receive(value, size, twi_stop);
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static void registerWrite(uint8_t reg, const T & value) {
    interface::send(reg);
    interface::send(value);
    sendTo();
  }

  /* ------------------------------------------------------------------------ */
  static inline void sendTo(twi_stop_t s = twi_stop) {
    interface::_sendTo(addr, s);
  }

  /* ------------------------------------------------------------------------ */
  static inline void receiveFrom(uint8_t size, twi_stop_t s = twi_stop) {
    interface::_receiveFrom(addr, size, s);
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline void send(const T & data, twi_stop_t s = twi_stop) {
    // pack the data
    interface::send(data);
    // send them
    sendTo(s);
  }

  template<typename T>
  static inline void send(const T data[], uint8_t size, twi_stop_t s = twi_stop) {
    // pack the data
    interface::send(data, size);
    // send them
    sendTo(s);
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline void receive(T & data, twi_stop_t s = twi_stop) {
    // get the data
    receiveFrom(sizeof(T), s);
    // unpack them
    interface::receive(data);
  }

  template<typename T>
  static inline void receive(T data[], uint8_t size, twi_stop_t s = twi_stop) {
    // get the data
    receiveFrom(size*sizeof(T), s);
    // unpack them
    interface::receive(data, size);
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline T transfer(const T & sdata, twi_stop_t s = twi_stop) {
    send(sdata, twi_non_stop);
    T rdata;
    receive(rdata, s);
    return rdata;
  }

  /* ------------------------------------------------------------------------ */
  template<typename T>
  static inline void transfer(const T sdata[], T rdata[],
			      uint8_t ssize, uint8_t rsize, twi_stop_t s = twi_stop) {
    send(sdata, ssize, twi_non_stop);
    receive(rdata, rsize, s);
  }
};


#endif //__TWI_HH__
