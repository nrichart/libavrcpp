#include "libavrc++.hh"


#include <components/leds/neo_pixel.hh>

typedef NeoPixel<pin13, 60> neo_pixel;
void setup(void) {
  neo_pixel::activate();

  for (int i = 0; i < 60; ++i) {
    neo_pixel::setLEDColor(i, Color<_rgb>(i, i, i));
  }
}

void loop(void) {
  neo_pixel::display();
}

