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

#include "rtc.hh"

const uint8_t DateTime::daysInMonth [] PROGMEM =
  { 31,28,31,30,31,30,31,31,30,31,30,31 };

static const char Jan[4] PROGMEM = "Jan";
static const char Feb[4] PROGMEM = "Feb";
static const char Mar[4] PROGMEM = "Mar";
static const char Apr[4] PROGMEM = "Apr";
static const char Mai[4] PROGMEM = "Mai";
static const char Jun[4] PROGMEM = "Jun";
static const char Jul[4] PROGMEM = "Jul";
static const char Aug[4] PROGMEM = "Aug";
static const char Sep[4] PROGMEM = "Sep";
static const char Oct[4] PROGMEM = "Oct";
static const char Nov[4] PROGMEM = "Nov";
static const char Dec[4] PROGMEM = "Dec";

PGM_P const DateTime::monthStr [12] PROGMEM = {
  Jan,
  Feb,
  Mar,
  Apr,
  Mai,
  Jun,
  Jul,
  Aug,
  Sep,
  Oct,
  Nov,
  Dec };

static const char Mon[4] PROGMEM = "Mon";
static const char Tue[4] PROGMEM = "Tue";
static const char Wed[4] PROGMEM = "Wed";
static const char Thu[4] PROGMEM = "Thu";
static const char Fri[4] PROGMEM = "Fri";
static const char Sat[4] PROGMEM = "Sat";
static const char Sun[4] PROGMEM = "Sun";

PGM_P const DateTime::dayStr [7] PROGMEM = {
  Mon,
  Tue,
  Wed,
  Thu,
  Fri,
  Sat,
  Sun };
