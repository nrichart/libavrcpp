#include <libavrc++.hh>
#include <components/sensors/bmp085.hh>

typedef BMP085<> bmp085;

void setup() {
  Serial0::activate(9600);
  bmp085::activate(bmp085::_ultrahighres);
}

void loop() {
  int32_t t = bmp085::getTemperature();
  int32_t p = bmp085::getPressure();
  Serial0::printf("Temp %ld - Pressure %ld\n\r", t, p);
  _delay_ms(1000);
}

