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

#ifndef __DS3234_HH__
#define __DS3234_HH__

#include "components/rtc/rtc.hh"
#include <spi/spi.hh>

template <class _spi = SPI0<ss>, typename int_sqw = unused_type> class DS3234 {
public:
  static void activate() {
    _spi::activate();
    _spi::setMode(spi::_mode_1);
    _spi::setBitOrder(spi::_bo_msb);

    _spi::select();
    _spi::transferByte(_write | _control);
    _spi::transferByte(0x00);
    _spi::unselect();

    _spi::select();
    _spi::transferByte(_write | _status);
    _spi::transferByte(0x00);
    _spi::unselect();

    _delay_ms(10);
  }

  static DateTime now() {
    uint8_t read[7] = {0};

    _spi::select();

    _spi::transferByte(_read | _seconds);

    read[_seconds] = bcd2bin(_spi::transferByte(0xFF) & 0x7F);
    read[_minutes] = bcd2bin(_spi::transferByte(0xFF) & 0x7F);
    read[_hours] = bcd2bin(_spi::transferByte(0xFF) & 0x3F);
    read[_day] = bcd2bin(_spi::transferByte(0xFF) & 0x07);
    read[_date] = bcd2bin(_spi::transferByte(0xFF) & 0x3F);
    read[_month] = bcd2bin(_spi::transferByte(0xFF) & 0xFF);
    read[_year] = bcd2bin(_spi::transferByte(0xFF) & 0xFF);

    _spi::unselect();

    uint8_t century = read[_month] / 80;
    read[_month] %= 80;
    read[_year] += 100 * (century);

    return DateTime(read[_year], read[_month], read[_date], read[_hours],
                    read[_minutes], read[_seconds], read[_day]);
  }

  static void adjust(DateTime &dt) {
    uint8_t year = dt.year() - 2000;

    _spi::select();

    _spi::transferByte(_write | _seconds);
    _spi::transferByte(bin2bcd(dt.second()));
    _spi::transferByte(bin2bcd(dt.minute()));
    _spi::transferByte(bin2bcd(dt.hour() & 0x3F));
    _spi::transferByte(bin2bcd(dt.dayOfWeek()));
    _spi::transferByte(bin2bcd(dt.day()));
    _spi::transferByte(bin2bcd(dt.month() + ((year / 100) * 80)));
    _spi::transferByte(bin2bcd(year % 100));

    _spi::unselect();
  }

private:
  enum command_t {
    _seconds = 0x00,
    _minutes = 0x01,
    _hours = 0x02,
    _day = 0x03,
    _date = 0x04,
    _month = 0x05,
    _year = 0x06,
    _a1_seconds = 0x07,
    _a1_minutes = 0x08,
    _a1_hours = 0x09,
    _a1_day = 0x0A,
    _a2_minutes = 0x0B,
    _a2_hours = 0x0C,
    _a2_day = 0x0D,
    _control = 0x0E,
    _status = 0x0F,
  };

  enum readwrite_t { _read = 0x00, _write = 0x80 };
};

#endif // __DS3234_HH__
