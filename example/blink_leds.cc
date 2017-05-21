#include "libavrc++.hh"

void setup() {
  pin13::output();
}

// the loop function runs over and over again forever
void loop() {
  pin13::high();
  _delay_ms(1000);              // wait for a second
  pin13::low();
  _delay_ms(1000);              // wait for a second
}
