#include "sw_serial.hh"
#include "common.hh"
#include "communication.hh"
#include "hw_serial.hh"

typedef SwSerial<pin3, pin2> SSerial;
//typedef HWSerial SSerial;


bool state;

ISR(INT0_vect)
{
  state ^= true;
  pin13::write(state);
  SwSerial<pin3, pin2>::storeReceive();
}

// ISR(PCINT2_vect) {
//   state ^= true;
//   pin13::write(state);
//   SSerial::storeReceive();
// }

void setup(void) {
  SSerial::activate(9600);
  HWSerial::activate(9600);
  pin13::output();

  pin2::PCIntOn();
}

void loop(void) {
  static int i = 0;
  pin13::high();
  //Communication<SSerial>::printf("%d ", Communication<SSerial>::available());
  //delay(500);
  //  pin13::low();
  //  Communication<SSerial>::printf("Low %d", i++);
  //  delay(500);
//  if(Communication<SSerial>::available()) {
//    char c[2];
//    c[0] = SSerial::receiveByte();
//    c[1] = 0;
//    Communication<HWSerial>::println(c);
//  }


//  if(Communication<HWSerial>::available()) {
//    char c[2];
//    c[0] = Communication<HWSerial>::receive<byte>();
//    c[1] = 0;
//    Communication<SSerial>::println(c);
//  }
}
