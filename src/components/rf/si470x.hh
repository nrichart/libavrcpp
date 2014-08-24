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

#include <avr/pgmspace.h>

#ifndef __SI470X_HH__
#define __SI470X_HH__

enum region_t {
  // De-Emphasis[4] Space[3:2] Band[1:0]
  EUROPE     = 0x14
  USA        = 0x00
  JAPAN_Wide = 0x15
  JAPAN      = 0x16
};

template< class rst, region_t region = EUROPE, class twi = TWI<0x10> >
class SI470x {
private:
  static const uint16_t spacing [] PROGMEM;
  static const uint16_t band_min[] PROGMEM;
  static const uint16_t band_max[] PROGMEM;

  enum register_t { // reorder the  register for the read  write operations real
		    // address are in the comment
    _statusrssi = 0x00,  // 0x0A
    _readchan   = 0x01,	 // 0x0B
    _rdsa       = 0x02,	 // 0x0C
    _rdsb       = 0x03,	 // 0x0D
    _rdsc       = 0x04,	 // 0x0E
    _rdsd       = 0x05,	 // 0x0F
    _deviceid   = 0x06,	 // 0x00
    _chipid     = 0x07,	 // 0x01
    _powercfg   = 0x08,	 // 0x02
    _channel    = 0x09,	 // 0x03
    _sysconfig1 = 0x0A,	 // 0x04
    _sysconfig2 = 0x0B,	 // 0x05
    _sysconfig3 = 0x0C,	 // 0x06
    _test1      = 0x0D,	 // 0x07
    _test2      = 0x0E,	 // 0x08
    _bootconfig = 0x0F };// 0x09


  template<register_t reg, uint8_t sbit, uint16_t mask>
  class Bit {
  public:
    enum {
      _register = reg
    };

    static set(uint16_t & reg_val, const uint16_t & val) {
      uint16_t wiped_bits = reg_val & (~(mask << sbit));
      reg_val = wiped_bits | ((val & mask) << sbit);
    }

    static uint16_t get(const uint16_t & reg_val, uint16_t val) {
      return (reg_val >> sbit) & mask;
    }
  };

  typedef Bit<_deviceid  , 12, 0x000F> _pn      ;
  typedef Bit<_deviceid  ,  0, 0x0FFF> _mfgid   ;
  typedef Bit<_chipid    , 10, 0x003F> _rev     ;
  typedef Bit<_chipid    ,  6, 0x000F> _dev     ;
  typedef Bit<_chipid    ,  0, 0x003F> _firmware;
  typedef Bit<_powercfg  , 15, 0x0001> _dsmute  ;
  typedef Bit<_powercfg  , 14, 0x0001> _dmute   ;
  typedef Bit<_powercfg  , 13, 0x0001> _mono    ;
  typedef Bit<_powercfg  , 11, 0x0001> _rdsml   ;
  typedef Bit<_powercfg  , 10, 0x0001> _skmode  ;
  typedef Bit<_powercfg  ,  9, 0x0001> _seekup  ;
  typedef Bit<_powercfg  ,  8, 0x0001> _seek    ;
  typedef Bit<_powercfg  ,  6, 0x0001> _disable ;
  typedef Bit<_powercfg  ,  0, 0x0001> _enable  ;
  typedef Bit<_channel   , 15, 0x0001> _tune    ;
  typedef Bit<_channel   ,  0, 0x03FF> _channel ;
  typedef Bit<_sysconfig1, 15, 0x0001> _rdsien  ;
  typedef Bit<_sysconfig1, 14, 0x0001> _stcien  ;
  typedef Bit<_sysconfig1, 12, 0x0001> _rds     ;
  typedef Bit<_sysconfig1, 11, 0x0001> _de      ;
  typedef Bit<_sysconfig1, 10, 0x0001> _abcd    ;
  typedef Bit<_sysconfig1,  6, 0x0003> _blndadj ;
  typedef Bit<_sysconfig1,  4, 0x0003> _gpio1   ;
  typedef Bit<_sysconfig1,  2, 0x0003> _gpio2   ;
  typedef Bit<_sysconfig1,  0, 0x0003> _gpio3   ;
  typedef Bit<_sysconfig2,  8, 0x00FF> _seekth  ;
  typedef Bit<_sysconfig2,  6, 0x0003> _band    ;
  typedef Bit<_sysconfig2,  4, 0x0003> _space   ;
  typedef Bit<_sysconfig2,  0, 0x000F> _volume  ;
  typedef Bit<_sysconfig3, 14, 0x0003> _smuter  ;
  typedef Bit<_sysconfig3, 12, 0x0003> _smutea  ;
  typedef Bit<_sysconfig3,  8, 0x0001> _volext  ;
  typedef Bit<_sysconfig3,  4, 0x000F> _sksnr   ;
  typedef Bit<_sysconfig3,  0, 0x000F> _skcnt   ;
  typedef Bit<_test1     , 15, 0x0001> _xoscen  ;
  typedef Bit<_test1     , 14, 0x0001> _ahizen  ;
  typedef Bit<_statusrssi, 15, 0x0001> _rdsr    ;
  typedef Bit<_statusrssi, 14, 0x0001> _stc     ;
  typedef Bit<_statusrssi, 13, 0x0001> _sfbl    ;
  typedef Bit<_statusrssi, 12, 0x0001> _afcrl   ;
  typedef Bit<_statusrssi, 11, 0x0001> _rdss    ;
  typedef Bit<_statusrssi,  9, 0x0003> _blera   ;
  typedef Bit<_statusrssi,  8, 0x0001> _st      ;
  typedef Bit<_statusrssi,  0, 0x00FF> _rssi    ;
  typedef Bit<_readchan  , 14, 0x0003> _blerb   ;
  typedef Bit<_readchan  , 12, 0x0003> _blerc   ;
  typedef Bit<_readchan  , 10, 0x0003> _blerd   ;
  typedef Bit<_readchan  ,  0, 0x03FF> _readchan;
  typedef Bit<_rdsa      ,  0, 0xFFFF> _rdsa    ;
  typedef Bit<_rdsb      ,  0, 0xFFFF> _rdsb    ;
  typedef Bit<_rdsc      ,  0, 0xFFFF> _rdsc    ;
  typedef Bit<_rdsd      ,  0, 0xFFFF> _rdsd    ;

  class registers {
  public:
    template<class bit>
    static set(const uint16_t & val) {
      bit::set(*(shadow + bit::_register), val);
    }

    template<class bit>
    static sbi() {
      bit::set(*(shadow + bit::_register), 0x1);
    }

    template<class bit>
    static cbi() {
      bit::set(*(shadow + bit::_register), 0x0);
    }


    template<class bit>
    static uint16_t get() {
      return bit::get(*(shadow + bit::_register));
    }

    static write(register_t last = _test2) {
      uint8_t n = last - _powercfg + 1;
      twi::send(shadow + _powercfg, n)
	}

    static read(register_t last = _bootconfig) {
      uint8_t n = last - _statusrssi + 1;
      twi::receive(shadow + _statusrssi, n)
	}

    static uint16_t shadow[16];
  };

public:
  enum speed_t {
    _fastest = 0x0,
    _fast    = 0x1,
    _slow    = 0x2,
    _slowest = 0x3 };

  enum attenuation_t {
    _16dB = 0,
    _14dB = 1,
    _12dB = 2,
    _10dB = 3 };

  enum seek_direction_t {
    _seek_down = 0,
    _seek_up   = 1,
  };

  enum seek_mode_t {
    seek_wrap  = 0,
    seek_limit = 1
  };

  static void activate() {
    rst::output();
    rst::low();
    _delay_ms(100);
    rst::high();
    _delay_ms(500);

    registers::read();

    registers::sbi<_xoscen>();
    registers::write(_test1);
    _delay_ms(50);

    registers::sbi<_enable>();
    registers::cbi<_disable>();
    registers::write(_powercfg);
    _delay_ms(500);

    registers::sbi<_rds>();

    mute(false);
    setSoftMute(false);
    setRegion(region, false);
    setVolume(volume, false);
    setFrequency(pgm_read_word(band_min + (region & 0x03)) / 100., false);

    registers::write(_sysconfig3);
  }

  // def hasRDS(self):
  //     registers::read(end = "statusrssi")
  //     return registers::get<_rdsr>() == 1

  // __rds = dict()
  // def pollRDS(self):
  //     '''
  //     The RDS part is experimental and not yet finished Reference is
  //     AN243 rev0.2 and RDBS Standard that is basically the same info
  //     '''
  //     start = time.time();
  //     registers::read(end = "rdsd")

        
  //     decoded = False
  //     if registers::get<_rdsr>():
  //         rdsa = registers::get<_rdsa>()
  //         rdsb = registers::get<_rdsb>()
  //         rdsc = registers::get<_rdsc>()
  //         rdsd = registers::get<_rdsd>()
  //         chwa = registers::get<_blera>()
  //         chwb = registers::get<_blerb>()
  //         chwc = registers::get<_blerc>()
  //         chwd = registers::get<_blerd>()

  //         if __freq in __rds:
  //             tmp_rds = copy.deepcopy(__rds[__freq])
  //         else:
  //             tmp_rds = RDS(region = __rds_region)
  //         decoded = tmp_rds.decode(rdsa, chwa,
  //                                  rdsb, chwb,
  //                                  rdsc, chwc,
  //                                  rdsd, chwd)
  //         if decoded:
  //             __rds[__freq] = tmp_rds

  //             if __debug:
  //                 print(__rds[__freq])
  //         else:
  //             if __debug:
  //                 print(("RDS status:" +
  //                        " A:{0} - B:{1} - C:{2} - C:{3}").format(__rds_errors[registers::get<_blera>()],
  //                                                                 __rds_errors[registers::get<_blerb>()],
  //                                                                 __rds_errors[registers::get<_blerc>()],
  //                                                                 __rds_errors[registers::get<_blerd>()]))

  //     dur = 0.086 - (time.time() - start)
  //     if(dur > 0):
  //         time.sleep(dur)

  //     if __freq in __rds:
  //         return __rds[__freq]
  //     else:
  //         return None


  static uint8_t getRSSI() {
    registers::read(_statusrssi);
    return (registers::get<_rssi>());
  }

  static void setSoftMute(bool mute = true,
			  atattenuation_t attenuation = _16, speed_t speed = _fastest,
			  bool direct_update = true) {
    registers::set<_dsmute>(mute ? 0x0 : 0x1);
    registers::set<_smutea>(attenuation);
    registers::set<_smuter>(speed);
    updateRegisters(direct_update, _sysconfig3);
  }

  /**
   * Set the channel according to Datasheet Rev 1.1 page 25
   * Freq (MHz) = Spacing (MHz) x Channel + 87.5 MHz
   * By default the station is tuned
   **/
  static void setFrequency(float frequency, bool tune = true, bool direct_update = true) {
    uint16_t freq = frequency * 100;
    uint16_t max = pgm_read_word(band_max + (region & 0x03));
    if (freq > max) freq = max;

    uint16_t channel = (freq - pgm_read_word(band_min + (region & 0x03))) /
      pgm_read_word(spacing + ((region >> 2) & 0x03));
    registers::set<_channel>(channel);

    if (tune) {
      registers::sbi<_tune>();
      registers::write(_channel);
      _delay_ms(60); // wait 60ms

      // wait that station is tuned
      waitSTC(1);

      registers::cbi<_tune>();
      registers::write(_channel);

      // clear the STC bit
      waitSTC(0);
    } else {
      updateRegisters(direct_update, _channel);
    }
  }

  static float getFrequency() {
    registers::read(_readchan);

    float freq = (pgm_read_word(band_min + (region & 0x03)) +
		  pgm_read_word(spacing + ((region >> 2) & 0x03)) * registers::get<_readchan>()) / 100.;
  }

  static void seek(seek_direction_t direction = seek_up,
		   seek_mode_t mode = seek_wrap,
		   uint8_t seek_rssi_threshold = 0x19,
		   uint8_t seek_snr_threshold  = 0x4,
		   uint8_t seek_fm_counts      = 0x8) {

    // the defaults values are the one recommanded in AN284
    registers::set<_seekth>(seek_rssi_threshold);
    registers::set<_sksnr>(seek_snr_threshold);
    registers::set<_skcnt>(seek_fm_counts);

    registers::set<_skmode>(mode);
    registers::set<_seekup>(direction);
    registers::sbi<_seek>();

    registers::write(_powercfg);
    waitSTC(1);

    if (registers::get<_sfbl>()) {
      return false;
    }

    registers::cbi<_seek>();
    registers::write(_powercfg);
    waitSTC(0);
    return true;
  }


  static void setRegion(region_t region, bool direct_update = true) {
    registers::set<_de>   ((region >> 4) & 0x1);
    registers::set<_space>((region >> 2) & 0x3);
    registers::set<_band> ( region       & 0x3);
    updateRegisters(direct_update, _sysconfig2);
  }

  /**
   * Set the volume between 0 and 31
   * 00: mute
   * 01: -58 dBFS
   * 0F: -30 dBFS
   * 10: -28 dBFS
   * 1E:   0 dBFS
   **/
  static void setVolume(uint8_t volume, bool direct_update = true) {
    if (volume > 0x1E) volume = 0x1E;

    uint8_t volext = (~volume >> 4) & 0x01;
    volume = (volume & 0xF) + ((volume >> 4) & 0x1);

    registers::set<_volume>(volume);
    registers::set<_volext>(volext);

    updateRegisters(direct_update, _sysconfig3);
  }

  static void mute(bool direct_update = true) {
    registers::set<_dmute>(~(registers::get<_dmute>()));
    updateRegisters(direct_update, _powercfg);
  }

private:
  static void waitSTC(uint8_t value) {
    do {
      registers::read(_statusrssi);
    } while(registers::get<_stc> != value);
  }

  static void updateRegisters(bool direct_update, register_t end) {
    if (direct_update) registers::write(end);
  }
};

template<class rst, region_t region, class twi>
uint16_t SI470x<rst, region, twi>::registers::shadow[16];

template<class rst, region_t region, class twi>
const uint16_t SI470x<rst, region, twi>::spacing [3] PROGMEM = { 20, 10, 5  }; // 100*MHz

template<class rst, region_t region, class twi>
const uint16_t SI470x<rst, region, twi>::band_min[3] PROGMEM = { 8750, 7600, 7600 }; // 100*MHz

template <typename T><class rst, region_t region, class twi>
const uint16_t SI470x<rst, region, twi>::band_max[3] PROGMEM = { 10800, 10800, 9000 };

#endif /* __SI470X_HH__ */
