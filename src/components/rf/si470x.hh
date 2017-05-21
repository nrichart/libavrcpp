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
#include "common/common.hh"
#include "components/rf/rds.hh"
/* -------------------------------------------------------------------------- */
#include <avr/pgmspace.h>
/* -------------------------------------------------------------------------- */

#ifndef __SI470X_HH__
#define __SI470X_HH__

#undef sbi
#undef cbi

/* -------------------------------------------------------------------------- */
enum region_t {
  // De-Emphasis[4] Space[3:2] Band[1:0]
  EUROPE = 0x14,
  USA = 0x00,
  JAPAN_Wide = 0x15,
  JAPAN = 0x16,
};

/* -------------------------------------------------------------------------- */
template <class rst, region_t region = EUROPE, class twi = TWI0<0x10>>
class SI470x {
private:
  static const uint16_t spacing[] PROGMEM;
  static const uint16_t band_min[] PROGMEM;
  static const uint16_t band_max[] PROGMEM;

  enum register_t { // reorder the  register for the read  write operations real
    // address are in the comment
    _statusrssi = 0x00, // 0x0A
    _readchan = 0x01,   // 0x0B
    _rdsa = 0x02,       // 0x0C
    _rdsb = 0x03,       // 0x0D
    _rdsc = 0x04,       // 0x0E
    _rdsd = 0x05,       // 0x0F
    _deviceid = 0x06,   // 0x00
    _chipid = 0x07,     // 0x01
    _powercfg = 0x08,   // 0x02
    _channel = 0x09,    // 0x03
    _sysconfig1 = 0x0A, // 0x04
    _sysconfig2 = 0x0B, // 0x05
    _sysconfig3 = 0x0C, // 0x06
    _test1 = 0x0D,      // 0x07
    _test2 = 0x0E,      // 0x08
    _bootconfig = 0x0F
  }; // 0x09

  template <register_t reg, uint8_t sbit, uint16_t mask> class Bit {
  public:
    enum { _register = reg };

    static void set(uint16_t &reg_val, const uint16_t &val) {
      uint16_t wiped_bits = reg_val & (~(mask << sbit));
      reg_val = wiped_bits | ((val & mask) << sbit);
    }

    static uint16_t get(const uint16_t &reg_val) {
      return (reg_val >> sbit) & mask;
    }
  };

  typedef Bit<_deviceid, 12, 0x000F> _pn;
  typedef Bit<_deviceid, 0, 0x0FFF> _mfgid;
  typedef Bit<_chipid, 10, 0x003F> _rev;
  typedef Bit<_chipid, 6, 0x000F> _dev;
  typedef Bit<_chipid, 0, 0x003F> _firmware;
  typedef Bit<_powercfg, 15, 0x0001> _dsmute;
  typedef Bit<_powercfg, 14, 0x0001> _dmute;
  typedef Bit<_powercfg, 13, 0x0001> _mono;
  typedef Bit<_powercfg, 11, 0x0001> _rdsm;
  typedef Bit<_powercfg, 10, 0x0001> _skmode;
  typedef Bit<_powercfg, 9, 0x0001> _seekup;
  typedef Bit<_powercfg, 8, 0x0001> _seek;
  typedef Bit<_powercfg, 6, 0x0001> _disable;
  typedef Bit<_powercfg, 0, 0x0001> _enable;
  typedef Bit<_channel, 15, 0x0001> _tune;
  typedef Bit<_channel, 0, 0x03FF> _channelb;
  typedef Bit<_sysconfig1, 15, 0x0001> _rdsien;
  typedef Bit<_sysconfig1, 14, 0x0001> _stcien;
  typedef Bit<_sysconfig1, 12, 0x0001> _rds;
  typedef Bit<_sysconfig1, 11, 0x0001> _de;
  typedef Bit<_sysconfig1, 10, 0x0001> _abcd;
  typedef Bit<_sysconfig1, 6, 0x0003> _blndadj;
  typedef Bit<_sysconfig1, 4, 0x0003> _gpio1;
  typedef Bit<_sysconfig1, 2, 0x0003> _gpio2;
  typedef Bit<_sysconfig1, 0, 0x0003> _gpio3;
  typedef Bit<_sysconfig2, 8, 0x00FF> _seekth;
  typedef Bit<_sysconfig2, 6, 0x0003> _band;
  typedef Bit<_sysconfig2, 4, 0x0003> _space;
  typedef Bit<_sysconfig2, 0, 0x000F> _volume;
  typedef Bit<_sysconfig3, 14, 0x0003> _smuter;
  typedef Bit<_sysconfig3, 12, 0x0003> _smutea;
  typedef Bit<_sysconfig3, 8, 0x0001> _volext;
  typedef Bit<_sysconfig3, 4, 0x000F> _sksnr;
  typedef Bit<_sysconfig3, 0, 0x000F> _skcnt;
  typedef Bit<_test1, 15, 0x0001> _xoscen;
  typedef Bit<_test1, 14, 0x0001> _ahizen;
  typedef Bit<_statusrssi, 15, 0x0001> _rdsr;
  typedef Bit<_statusrssi, 14, 0x0001> _stc;
  typedef Bit<_statusrssi, 13, 0x0001> _sfbl;
  typedef Bit<_statusrssi, 12, 0x0001> _afcrl;
  typedef Bit<_statusrssi, 11, 0x0001> _rdss;
  typedef Bit<_statusrssi, 9, 0x0003> _blera;
  typedef Bit<_statusrssi, 8, 0x0001> _st;
  typedef Bit<_statusrssi, 0, 0x00FF> _rssi;
  typedef Bit<_readchan, 14, 0x0003> _blerb;
  typedef Bit<_readchan, 12, 0x0003> _blerc;
  typedef Bit<_readchan, 10, 0x0003> _blerd;
  typedef Bit<_readchan, 0, 0x03FF> _readchanb;
  typedef Bit<_rdsa, 0, 0xFFFF> _rdsab;
  typedef Bit<_rdsb, 0, 0xFFFF> _rdsbb;
  typedef Bit<_rdsc, 0, 0xFFFF> _rdscb;
  typedef Bit<_rdsd, 0, 0xFFFF> _rdsdb;

  class registers {
  public:
    template <class bit> static void set(const uint16_t &val) {
      bit::set(*(shadow + bit::_register), val);
    }

    template <class bit> static void sbi() {
      bit::set(*(shadow + bit::_register), 0x1);
    }

    template <class bit> static void cbi() {
      bit::set(*(shadow + bit::_register), 0x0);
    }

    template <class bit> static uint16_t get() {
      return bit::get(*(shadow + bit::_register));
    }

    static void write(register_t last = _test2) {
      uint8_t n = last - _powercfg + 1;
      twi::send(shadow + _powercfg, n);
    }

    static void read(register_t last = _bootconfig) {
      uint8_t n = last - _statusrssi + 1;
      twi::receive(shadow + _statusrssi, n);
    }

    static uint16_t shadow[16];
  };

public:
  enum speed_t { _fastest = 0x0, _fast = 0x1, _slow = 0x2, _slowest = 0x3 };

  enum attenuation_t { _16dB = 0, _14dB = 1, _12dB = 2, _10dB = 3 };

  enum seek_direction_t {
    _seek_down = 0,
    _seek_up = 1,
  };

  enum seek_mode_t { _seek_wrap = 0, _seek_limit = 1 };

  static void activate() {
    rst::output();
    sda::output();
    sda::low();

    rst::low();
    _delay_ms(100);
    rst::high();
    _delay_ms(500);

    twi::activate();

    registers::read();

    registers::template sbi<_xoscen>();
    registers::write(_test1);
    _delay_ms(500);

    registers::template sbi<_enable>();
    registers::template cbi<_disable>();
    registers::write(_powercfg);
    _delay_ms(500);

    registers::template sbi<_rds>();
    registers::template sbi<_rdsm>();

    mute(false);
    setSoftMute(false);
    setRegion(region, false);
    setVolume(16, false);
    setFrequency(pgm_read_word(band_min + (region & 0x03)) / 100., false);

    registers::write(_sysconfig3);
  }

  template <class comm> static void printRegisters() {
    for (int i = 0; i < _bootconfig; ++i) {
      comm::printf("Reg 0x%x = 0x%x\n\r", (i + 0x0A) % 0x0F,
                   registers::shadow[i]);
    }
  }

  static bool hasRDS() {
    registers::read(_statusrssi);
    return (registers::template get<_rdsr>() == 1);
  }

  static void decodeRDS(RDS &rds) {
    registers::read(_statusrssi);
    if (registers::template get<_rdsr>()) {
      registers::read(_rdsd);
      if (registers::template get<_blera>() < 2) {
        uint16_t rdsa = registers::template get<_rdsab>();
        uint16_t rdsb = registers::template get<_rdsbb>();
        uint16_t rdsc = registers::template get<_rdscb>();
        uint16_t rdsd = registers::template get<_rdsdb>();
        uint8_t checkwords = registers::template get<_blerd>();
        checkwords <<= 2;
        checkwords |= registers::template get<_blerc>();
        checkwords <<= 2;
        checkwords |= registers::template get<_blerb>();

        rds.decode(rdsa, rdsb, rdsc, rdsd, checkwords);
      }
    }
  }

  static uint8_t getRSSI() {
    registers::read(_statusrssi);
    return (registers::template get<_rssi>());
  }

  static void setSoftMute(bool mute = true, attenuation_t attenuation = _16dB,
                          speed_t speed = _fastest, bool direct_update = true) {
    registers::template set<_dsmute>(mute ? 0x0 : 0x1);
    registers::template set<_smutea>(attenuation);
    registers::template set<_smuter>(speed);
    updateRegisters(direct_update, _sysconfig3);
  }

  /**
   * Set the channel according to Datasheet Rev 1.1 page 25
   * Freq (MHz) = Spacing (MHz) x Channel + 87.5 MHz
   * By default the station is tuned
   **/
  static void setFrequency(float frequency, bool tune = true,
                           bool direct_update = true) {
    uint16_t freq = frequency * 100;
    uint16_t max = pgm_read_word(band_max + (region & 0x03));
    if (freq > max)
      freq = max;

    uint16_t _band_min = pgm_read_word(band_min + (region & 0x03));
    uint16_t _spacing = pgm_read_word(spacing + ((region >> 2) & 0x03));

    uint16_t channel = (freq - _band_min) / _spacing;
    registers::template set<_channelb>(channel);

    if (tune) {
      registers::template sbi<_tune>();
      registers::write(_channel);
      _delay_ms(60); // wait 60ms

      // wait that station is tuned
      waitSTC(1);

      registers::template cbi<_tune>();
      registers::write(_channel);

      // clear the STC bit
      waitSTC(0);
    } else {
      updateRegisters(direct_update, _channel);
    }
  }

  static float getFrequency() {
    registers::read(_readchan);

    uint16_t _band_min = pgm_read_word(band_min + (region & 0x03));
    uint16_t _spacing = pgm_read_word(spacing + ((region >> 2) & 0x03));
    uint16_t channel = registers::template get<_readchanb>();
    float freq = (_band_min + channel * _spacing) / 100.;
    return freq;
  }

  static bool seek(seek_direction_t direction = _seek_up,
                   seek_mode_t mode = _seek_wrap,
                   uint8_t seek_rssi_threshold = 0x19,
                   uint8_t seek_snr_threshold = 0x4,
                   uint8_t seek_fm_counts = 0x8) {

    // the defaults values are the one recommanded in AN284
    registers::template set<_seekth>(seek_rssi_threshold);
    registers::template set<_sksnr>(seek_snr_threshold);
    registers::template set<_skcnt>(seek_fm_counts);

    registers::template set<_skmode>(mode);
    registers::template set<_seekup>(direction);
    registers::template sbi<_seek>();

    registers::write(_powercfg);
    waitSTC(1);

    if (registers::template get<_sfbl>()) {
      return false;
    }

    registers::template cbi<_seek>();
    registers::write(_powercfg);
    waitSTC(0);
    return true;
  }

private:
  static void setRegion(region_t reg, bool direct_update = true) {
    registers::template set<_de>((reg >> 4) & 0x1);
    registers::template set<_space>((reg >> 2) & 0x3);
    registers::template set<_band>(reg & 0x3);
    updateRegisters(direct_update, _sysconfig2);
  }

public:
  /**
   * Set the volume between 0 and 31
   * 00: mute
   * 01: -58 dBFS
   * 0F: -30 dBFS
   * 10: -28 dBFS
   * 1E:   0 dBFS
   **/
  static void setVolume(uint8_t volume, bool direct_update = true) {
    if (volume > 0x1E)
      volume = 0x1E;

    uint8_t volext = (~volume >> 4) & 0x01;
    volume = (volume & 0xF) + ((volume >> 4) & 0x1);

    registers::template set<_volume>(volume);
    registers::template set<_volext>(volext);

    updateRegisters(direct_update, _sysconfig3);
  }

  static void mute(bool direct_update = true) {
    registers::template set<_dmute>(~(registers::template get<_dmute>()));
    updateRegisters(direct_update, _powercfg);
  }

private:
  static void waitSTC(uint8_t value) {
    do {
      registers::read(_statusrssi);
    } while (registers::template get<_stc>() != value);
  }

  static void updateRegisters(bool direct_update, register_t end) {
    if (direct_update)
      registers::write(end);
  }
};

template <class rst, region_t region, class twi>
uint16_t SI470x<rst, region, twi>::registers::shadow[16];

template <class rst, region_t region, class twi>
const uint16_t SI470x<rst, region, twi>::spacing[3] PROGMEM = {20, 10,
                                                               5}; // 100*MHz

template <class rst, region_t region, class twi>
const uint16_t SI470x<rst, region, twi>::band_min[3] PROGMEM = {
    8750, 7600, 7600}; // 100*MHz

template <class rst, region_t region, class twi>
const uint16_t SI470x<rst, region, twi>::band_max[3] PROGMEM = {10800, 10800,
                                                                9000};

#endif /* __SI470X_HH__ */
