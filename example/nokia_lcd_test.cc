#include "libavrc++.hh"
#include "components/lcd/pcd8544.hh"
#include "components/lcd/glcd.hh"

typedef gLCD<PCD8544<pin7, pin6, pin5>> lcd;

void setup(void) {
  lcd::activate();
  lcd::setBacklight(128);
}

void loop(void) {
  lcd::clear();
  lcd::print(0, 0, "Hello World!!");

  for (uint8_t i = 0; i < lcd::width; ++i) {
    lcd::setPixel(12, i);
  }
  _delay_ms(2000);
}
