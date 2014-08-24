#include "adc.hh"
#include "common.hh"
#include "looper.hh"
#include "spi.hh"
#include "queue.hh"

// Slave
float sampling_freq = 1;
//Looper looper(int(1000/sampling_freq));
Looper looper(1000);

SPI spi;

#define SS_LOW()				\
  cbi(PORT_SPI, PORT_SS);			\
  cbi(PORTD, PORTD2)

#define SS_HIGH()				\
  sbi(PORT_SPI, PORT_SS);			\
  sbi(PORTD, PORTD2)


void setup() {
  Serial.begin(57600);
  sbi(DDRD, DDD2);

  SS_HIGH();

  spi.init(SPI_MASTER);

  Serial.println("Ready to go!!!");
}

bool started = false;

void loop() {
  looper.waitNext();

  if(Serial.available() > 0) {
    int command = Serial.read();
    switch(command) {
    case 's': started = true; break; 
    case 'h': started = false; break;
    }
  }

  if(started) {
    Serial.println("Reading data...");
    uint16_t count, temp;

    SS_LOW();

    count = spi.transfer<uint16_t>(0);
    temp  = spi.transfer<uint16_t>(0);

    SS_HIGH();

    Serial.print(count);
    Serial.println(temp);
    Serial.println("Data read.");
  } else {
    Serial.println("Waiting to start");
  }
}
