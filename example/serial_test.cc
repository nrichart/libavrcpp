#include <libavrc++.hh>

void setup() {
  Serial0::activate(9600);
  Serial0::println("Hello World");
}

void loop() {
  static uint8_t i = 0;
  Serial0::printf("Hello world %d\r\n", i++);
  _delay_ms(500);
}
