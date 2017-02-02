#include <libavrc++.hh>
#include <components/leds/neo_pixel.hh>

typedef NeoPixel<pin8, 60> neo;

void setup(void) {
  Serial0::activate(9600);
  Serial0::println("Hello World");

  neo::activate();
  neo::setBrightness(20);
  neo::display();
}

void loop() {
  for(uint8_t i=0; i < 60; i++) {
    Color<_rgb> color((i * 255 / 60) % 60, i, i);

    neo::setLEDColor(i, color);
  }
  neo::display();
}
