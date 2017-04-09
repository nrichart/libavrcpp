#include <libavrc++.hh>

#include <components/rtc/ds3234.hh>
#include <components/rtc/rtc.hh>

typedef RTC< DS3234<> > rtc;

void setup(void) {
  Serial0::activate(9600);
  rtc::activate();
  DateTime dt(2014, 8, 16, 04, 16, 0);
  rtc::adjust(dt);
}

void loop(void) {
  Serial0::println(rtc::now().str());
  _delay_ms(1000);
}
