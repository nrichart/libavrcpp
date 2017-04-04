#include <libavrc++.hh>

#include <components/lcd/st7565.hh>
#include <components/lcd/pcd8544.hh>
#include <components/lcd/glcd.hh>

#include <avr/pgmspace.h>

const  uint8_t logo16_glcd_bmp[] PROGMEM = {
0x30, 0xf0, 0xf0, 0xf0, 0xf0, 0x30, 0xf8, 0xbe, 0x9f, 0xff, 0xf8, 0xc0, 0xc0, 0xc0, 0x80, 0x00,
0x20, 0x3c, 0x3f, 0x3f, 0x1f, 0x19, 0x1f, 0x7b, 0xfb, 0xfe, 0xfe, 0x07, 0x07, 0x07, 0x03, 0x00, };

typedef RGBLed<pin3, pin5, pin6> bck_light;
typedef gLCD<ST7565<pin4, pin2, bck_light, pin7>> lcd;


void setup(void) {
  lcd::activate();

  lcd::print(0, 0, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam. tototototototo");
  lcd::drawBitmap(0, 0, logo16_glcd_bmp, 16, 16);
  lcd::display();

  _delay_ms(1000);
  lcd::clear();
  lcd::drawRectangle(10, 10, 120, 50, 1, true);
  lcd::display();

  _delay_ms(1000);
  lcd::clear();
  lcd::drawCircle(64, 32, 10, 1, true);
  lcd::display();

  for(uint8_t y = 0; y < lcd::width; ++y) {
    lcd::setPixel(y, y % lcd::height);
  }
  lcd::display();

  lcd::print(0, 4, "-------------------");
  lcd::display();

  // for(uint8_t y = 0; y < lcd::width; ++y) {
  //   lcd::setPixel(y, lcd::height - y % lcd::height);
  //   lcd::display();
  // }
}

void loop(void) {
}
