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
   Original code TinyRTC
   Code by JeeLabs http://news.jeelabs.org/code/
   Released to the public domain! Enjoy!
   modified by netWorms for libavrc++
*/

#ifndef __RTC_HH__
#define __RTC_HH__


#include<stdio.h>
#include<string.h>
#include<avr/pgmspace.h>

#define SECONDS_FROM_1970_TO_2000 946684800

/* -------------------------------------------------------------------------- */
inline uint8_t bcd2bin(uint8_t val) {
  return ((val & 0x0F) + 10 * ((val) >> 4));
}
inline uint8_t bin2bcd(uint8_t val) {
  return ((val % 10) + (((val) / 10) << 4));
}

/* -------------------------------------------------------------------------- */
class DateTime {
public:
  DateTime(uint32_t t = 0) {
    this->ss = t % 60; t /= 60;
    this->mm = t % 60; t /= 60;
    this->hh = t % 24; t /= 24;
    uint16_t days = t;
    uint8_t leap;
    for (this->yOff = 0; ; ++this->yOff) {
      leap = this->yOff % 4 == 0;
      if (days < 365u + leap)
	break;
      days -= 365u + leap;
    }
    for (m = 1; ; ++m) {
      uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
      if (leap && m == 2)
	++daysPerMonth;
      if (days < daysPerMonth)
	break;
      days -= daysPerMonth;
    }
    this->d = days + 1;
  }

  DateTime(uint16_t year, uint8_t month, uint8_t day,
	   uint8_t hour =0, uint8_t min =0, uint8_t sec =0,
	   int8_t day_of_week = -1) {
    if (year >= 2000)
      year -= 2000;
    this->yOff = year;
    this->m    = month;
    this->d    = day;
    this->hh   = hour;
    this->mm   = min;
    this->ss   = sec;
    if(day_of_week == -1) {
      this->dow = _dayOfWeek();
    } else {
      this->dow = day_of_week;
    }
  }

  DateTime(const char* date, const char* time) {
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    yOff = conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0]) {
    case 'J': this->m = (date[1] == 'a' ? 1 : (date[2] == 'n' ? 6 : 7)); break;
    case 'F': this->m = 2; break;
    case 'A': this->m = date[2] == 'r' ? 4 : 8; break;
    case 'M': this->m = date[2] == 'r' ? 3 : 5; break;
    case 'S': this->m = 9; break;
    case 'O': this->m = 10; break;
    case 'N': this->m = 11; break;
    case 'D': this->m = 12; break;
    }

    this->d  = conv2d(date + 4);
    this->hh = conv2d(time);
    this->mm = conv2d(time + 3);
    this->ss = conv2d(time + 6);
  }

  char * date_str() {
    static char _str[16];
    char ms[4] = { 0 };
    char ds[4] = { 0 };
    if(this->m > 12) strncpy(ms, "NaM", 4);
    else
      strncpy_PF(ms, (pgm_read_word(monthStr + this->m   - 1)), 4);

    if(this->dow > 7) sprintf(ds, "%3u", this->dow);
    else
      strncpy_PF(ds, (pgm_read_word(dayStr   + this->dow - 1)), 4);
    sprintf(_str, "%3s %3s %2hhu %4hu",
	    ds, ms, this->d, (this->yOff+2000));
    return _str;
  }

  char * time_str() {
    static char _str[9];
    sprintf(_str, "%02hhu:%02hhu:%02hhu",
	    this->hh, this->mm, this->ss);
    return _str;
  }

  char * str() {
    static char _str[25];
    sprintf(_str, "%s %s",date_str(), time_str());
    return _str;
  }


  uint16_t year()      const { return 2000 + this->yOff; }
  uint8_t  month()     const { return this->m; }
  uint8_t  day()       const { return this->d; }
  uint8_t  hour()      const { return this->hh; }
  uint8_t  minute()    const { return this->mm; }
  uint8_t  second()    const { return this->ss; }
  uint8_t  dayOfWeek() const { return this->dow; }

  // 32-bit times as seconds since 1/1/2000
  long secondstime() const {
    uint16_t days = date2days(this->yOff, this->m, this->d);
    long t = time2long(days, this->hh, this->mm, this->ss);
    return t;
  }

  // 32-bit times as seconds since 1/1/1970
  uint32_t unixtime(void) const {
    uint16_t days = date2days(this->yOff, this->m, this->d);
    uint32_t t = time2long(days, this->hh, this->mm, this->ss);
    t += SECONDS_FROM_1970_TO_2000;  // seconds from 1970 to 2000
    return t;
  }

protected:
  uint8_t yOff, m, d, hh, mm, ss, dow;

  static const uint8_t daysInMonth [] PROGMEM;
  static PGM_P const monthStr [] PROGMEM;
  static PGM_P const dayStr [] PROGMEM;


  /* ------------------------------------------------------------------------ */
  uint8_t  _dayOfWeek() const {
    uint16_t day = date2days(yOff, m, d);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
  }

  static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
      v = *p - '0';
    return 10 * v + *++p - '0';
  }

  // number of days since 2000/01/01, valid for 2001..2099
  static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000)
      y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
      days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0)
      ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
  }

  static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
    return ((days * 24L + h) * 60 + m) * 60 + s;
  }
};

/* -------------------------------------------------------------------------- */
template<class hw>
class RTC : public hw {
public:
  static void activate() { hw::activate(); }
};

#endif // __RTC_HH__
