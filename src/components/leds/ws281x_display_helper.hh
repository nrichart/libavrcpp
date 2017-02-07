/*
  This file is part of the Adafruit NeoPixel library.

  NeoPixel is free software: you can  redistribute it and/or modify it under the
  terms  of the  GNU Lesser  General  Public License  as published  by the  Free
  Software Foundation, either version 3 of  the License, or (at your option) any
  later version.

  NeoPixel is distributed  in the hope that  it will be useful,  but WITHOUT ANY
  WARRANTY; without even the implied  warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR  PURPOSE.  See  the GNU  Lesser General  Public License  for more
  details.

  You should have received a copy of the GNU Lesser General Public License along
  with NeoPixel.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
   This code is an adaptation of the Adafruit NeoPixel library for Arduino.

   - Originally  written  by Phil  Burgess  /  Paint  Your Dragon  for  Adafruit
     Industries,  contributions by  PJRC and  other members  of the  open source
     community.

   - Modified by netWorms to integrate in libAVR++
 */

#ifndef __WS281X_DISPLAY_HELPER_HH__
#define __WS281X_DISPLAY_HELPER_HH__

template < typename pin, ws281x_drive_freq_t drive_freq = _ws281x_800_khz >
struct WS281xDisplayHelper {
};

template< typename pin >
struct WS281xDisplayHelper<pin, _ws281x_800_khz> {
  static void display(uint8_t * rgb, uint8_t nb_leds) {
    volatile uint16_t nb_colors = 3 * nb_leds;

    typedef typename pin::port _port;
    volatile uint8_t * leds = rgb;
    volatile uint8_t * port = _port::ptr();

    const uint8_t pin_bit = pin::bit;
    volatile uint8_t hi = _port::val() | (1 << pin_bit);
    volatile uint8_t lo = _port::val() & ~(1 << pin_bit);

    volatile uint8_t byte = *leds++;

    cli();

#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)
    volatile uint8_t n1 = 0, n2 = 0;
    n1 = lo;
    if(byte & 0x80) n1 = hi;

    asm volatile
      (
       "loop:"                   "\n\t" // Clk  Pseudocode
       // Bit 7:
       "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
       "mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
       "out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
       "rjmp .+0"                "\n\t" // 2    nop nop
       "sbrc %[byte] , 6"        "\n\t" // 1-2  if(b & 0x40)
       "mov  %[n2]   , %[hi]"    "\n\t"  // 0-1   n2 = hi
       "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
       "rjmp .+0"                "\n\t" // 2    nop nop
       // Bit 6:
       "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
       "mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
       "out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
       "rjmp .+0"                "\n\t" // 2    nop nop
       "sbrc %[byte] , 5"        "\n\t" // 1-2  if(b & 0x20)
       "mov  %[n1]   , %[hi]"    "\n\t"  // 0-1   n1 = hi
       "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
       "rjmp .+0"                "\n\t" // 2    nop nop
       // Bit 5:
       "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
       "mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
       "out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
       "rjmp .+0"                "\n\t" // 2    nop nop
       "sbrc %[byte] , 4"        "\n\t" // 1-2  if(b & 0x10)
       "mov  %[n2]   , %[hi]"    "\n\t"  // 0-1   n2 = hi
       "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
       "rjmp .+0"                "\n\t" // 2    nop nop
       // Bit 4:
       "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
       "mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
       "out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
       "rjmp .+0"                "\n\t" // 2    nop nop
       "sbrc %[byte] , 3"        "\n\t" // 1-2  if(b & 0x08)
       "mov  %[n1]   , %[hi]"    "\n\t"  // 0-1   n1 = hi
       "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
       "rjmp .+0"                "\n\t" // 2    nop nop
       // Bit 3:
       "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
       "mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
       "out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
       "rjmp .+0"                "\n\t" // 2    nop nop
       "sbrc %[byte] , 2"        "\n\t" // 1-2  if(b & 0x04)
       "mov  %[n2]   , %[hi]"    "\n\t"  // 0-1   n2 = hi
       "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
       "rjmp .+0"                "\n\t" // 2    nop nop
       // Bit 2:
       "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
       "mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
       "out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
       "rjmp .+0"                "\n\t" // 2    nop nop
       "sbrc %[byte] , 1"        "\n\t" // 1-2  if(b & 0x02)
       "mov  %[n1]   , %[hi]"    "\n\t"  // 0-1   n1 = hi
       "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
       "rjmp .+0"                "\n\t" // 2    nop nop
       // Bit 1:
       "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
       "mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
       "out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
       "rjmp .+0"                "\n\t" // 2    nop nop
       "sbrc %[byte] , 0"        "\n\t" // 1-2  if(b & 0x01)
       "mov  %[n2]   , %[hi]"    "\n\t"  // 0-1   n2 = hi
       "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
       "sbiw %[count], 1"        "\n\t" // 2    i-- (don't act on Z flag yet)
       // Bit 0:
       "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
       "mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
       "out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
       "ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++
       "sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 0x80)
       "mov  %[n1]   , %[hi]"    "\n\t"  // 0-1   n1 = hi
       "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
       "brne loop"              "\n"    // 2    while(i) (Z flag set above)
       : [byte]  "+r" (byte),
	 [n1]    "+r" (n1),
	 [n2]    "+r" (n2),
	 [count] "+w" (nb_colors)
       : [port]   "I" (_SFR_IO_ADDR(*port)),
	 [ptr]    "e" (leds),
	 [hi]     "r" (hi),
	 [lo]     "r" (lo)
       );
#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)
    volatile uint8_t tmp = lo,  bit = 8;

    asm volatile
      (
       "loop:"                    "\n\t" // Clk  Pseudocode    (T =  0)
       "st   %[port],  %[hi]"     "\n\t" // 2    PORT = hi     (T =  2)
       "sbrc %[byte],  7"         "\n\t" // 1-2  if(b & 128)
       "mov  %[tmp],   %[hi]"     "\n\t" // 0-1   tmp = hi     (T =  4)
       "dec  %[bit]"              "\n\t" // 1    bit--         (T =  5)
       "st   %[port],  %[tmp]"    "\n\t" // 2    PORT = tmp    (T =  7)
       "mov  %[tmp],   %[lo]"     "\n\t" // 1    next = lo     (T =  8)
       "breq incleds"             "\n\t" // 1-2  if(bit == 0)
       "lsl  %[byte]"             "\n\t" // 1    b <<= 1       (T = 10)
       "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 12)
       "nop"                      "\n\t" // 1    nop           (T = 13)
       "st   %[port],  %[lo]"     "\n\t" // 2    PORT = lo     (T = 15)
       "nop"                      "\n\t" // 1    nop           (T = 16)
       "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 18)
       "rjmp loop"                "\n\t" // 2    -> head20 (next bit out)
       "incleds:"                 "\n\t" //                    (T = 10)
       "ldi  %[bit],   8"         "\n\t" // 1    bit = 8       (T = 11)
       "ld   %[byte],  %a[leds]+" "\n\t" // 2    b = *ptr++    (T = 13)
       "st   %[port],  %[lo]"     "\n\t" // 2    PORT = lo     (T = 15)
       "nop"                      "\n\t" // 1    nop           (T = 16)
       "subi %[count], 1"         "\n\t" // 1    i--           (T = 18)
       "nop"                      "\n\t" //
       "brne loop"                "\n"   // 2    if(i != 0) -> (next byte)
       : [port]  "+e" (port),
	 [byte]  "+r" (byte),
	 [bit]   "+r" (bit),
	 [tmp]   "+r" (tmp),
	 [count] "+w" (nb_colors)
       : [leds]  "e"  (leds),
	 [hi]    "r"  (hi),
	 [lo]    "r"  (lo)
       );
#else
#error "Not implemented"
#endif

    sei();
  }
};

#endif /* __WS281X_DISPLAY_HELPER_HH__ */
