set(CMAKE_TOOLCHAIN_FILE ../libavrc++Toolchain.cmake)

project(libavrc++_examples)

libavrcpp_list_configs()

cmake_minimum_required(VERSION 2.6)

#add_avr_firmware(blinky CONFIG arduino_pro VARIANT 16MHz_5V_328p  PORT /dev/ttyUSB
add_avr_firmware(blinky CONFIG trinket_3v PORT /dev/ttyUSB0
  blink_leds.cc
  )

add_avr_firmware(nokia_lcd CONFIG arduino_pro VARIANT 8MHz_3.3V_328p PORT /dev/ttyUSB0
  nokia_lcd_test.cc
  )

add_avr_firmware(st7565 CONFIG arduino_pro VARIANT 8MHz_3.3V_328p PORT /dev/ttyUSB0
  st7565_test.cc
  )

add_avr_firmware(rtc CONFIG arduino_pro VARIANT 8MHz_3.3V_328p PORT /dev/ttyUSB0
  rtc_ds3234.cc
  )

add_avr_firmware(serial CONFIG arduino_pro VARIANT 8MHz_3.3V_328p PORT /dev/ttyUSB0
  serial_test.cc
  )

add_avr_firmware(sws CONFIG arduino_pro VARIANT 8MHz_3.3V_328p PORT /dev/ttyUSB0
  sws_test.cc
  )

add_avr_firmware(bmp085_test CONFIG arduino_pro VARIANT 8MHz_3.3V_328p PORT /dev/ttyUSB0
  bmp085_test.cc
  )

add_avr_firmware(neo_ring_test CONFIG arduino_pro VARIANT 8MHz_3.3V_328p PORT /dev/ttyUSB0
  neo_ring_test.cc
  )
