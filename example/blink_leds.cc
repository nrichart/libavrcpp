#include "libavrc++.hh"

void setup() {
  pin1::output();
}

// the loop function runs over and over again forever
void loop() {
  pin1::high();
  _delay_ms(100);              // wait for a second
  pin1::low();
  _delay_ms(1000);              // wait for a second
}
