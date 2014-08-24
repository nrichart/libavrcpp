#include <libavrc++.hh>

void setup(void) {
  pin9::output();
  pin3::output();
}

void loop(void) {
  static uint8_t val = 0;
  val != val;

  if(val)
    pin9::high();
  else
    pin9::low();

  _delay_ms(500);
}
