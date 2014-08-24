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
A2D adc;

void setup() {
  Serial.begin(57600);
  sbi(DDRD, DDD2);

  spi.init(SPI_SLAVE);
  adc.init(ADC_PRESCALER_64);

  Serial.println("Ready to go!!!");
}

#define MAX_CACHE_SIZE 100

Queue<uint16_t, MAX_CACHE_SIZE> cache;

uint16_t count = 0, send_count = 0;

void loop() {
  //looper.waitNext();

  int temp = adc.read(0);

  Serial.print(count++);
  Serial.print(" ");
  Serial.println(temp);

  // if(cache.isFull()) {
  //   cbi(PORTD, PORTD2);
  // } else {
  //   cache.push(temp);
  //   sbi(PORTD, PORTD2);
  // }


  Serial.println(spi.isSelected());
  if(spi.isSelected()) {
    Serial.println("Sending data...");
    spi.send<uint16_t>(send_count++);
    spi.send<uint16_t>(temp);
    Serial.println("Data sent.");
  }
  // while(spi.isSelected() && !cache.isEmpty()) {
  //   spi.send<uint16_t>(send_count++);
  //   spi.send<uint16_t>(cache.pop());
  // }

  // if(spi.isSelected() && cache.isEmpty())
  //   Serial.println("Data sent.");
  // else
  //   Serial.println("Caching Data");
  

}
