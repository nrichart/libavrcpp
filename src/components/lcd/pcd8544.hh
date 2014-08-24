/**
   This file is part of libavrc++.

   Copyright (c) 2014, netWorms, All rights reserved.

   libavrc++ is  free software: you can  redistribute it and/or modify  it under
   the terms of the  GNU Lesser General Public License as  published by the Free
   Software Foundation, either version 3 of the License, or (at your option) any
   later version.

   libavrc++ is distributed in the hope that  it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR  PURPOSE.  See the  GNU Lesser  General Public License  for more
   details.

   You should  have received  a copy  of the GNU  Lesser General  Public License
   along with libavrc++.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PCD8544_HH__
#define __PCD8544_HH__

#include "common/common.hh"


#define PCD8544_LCD_COMMAND 0x0
#define PCD8544_LCD_DATA    0x1
/* -------------------------------------------------------------------------- */
/* Normal instruction set                                                     */
/* -------------------------------------------------------------------------- */
#define PCD8544_LCD_DISP_CTRL 0x08
/*
 * D | E |
 * 0 | 0 | display blank
 * 1 | 0 | normal mode
 * 0 | 1 | all display segments on
 * 1 | 1 | inverse video mode
 */
#define PCD8544_LCD_DISP_CTRL_D    0x04
#define PCD8544_LCD_DISP_CTRL_E    0x01

#define PCD8544_LCD_CMD_X          0x80
#define PCD8544_LCD_CMD_Y          0x40

/* -------------------------------------------------------------------------- */
/* Extended instruction set                                                   */
/* -------------------------------------------------------------------------- */
#define PCD8544_LCD_VOP       0x80
#define PCD8544_LCD_BIAS      0x10
#define PCD8544_LCD_TEMP_COEF 0x04
/* TC1 | TC0 |
 *  0  |  0  |  VLCD temperature coefficient 0
 *  0  |  1  |  VLCD temperature coefficient 1
 *  1  |  0  |  VLCD temperature coefficient 2
 *  1  |  1  |  VLCD temperature coefficient 3
 */

#define PCD8544_LCD_PDHV    0x20
/*
 * BIT |            0           |              1
 * ----------------------------------------------------------------
 * PD  | chip is active         | chip is in Power-down mode
 * H   | horizontal addressing  | vertical addressing
 * V   | use basic instruction  | set use extended instruction set
 */
#define PCD8544_LCD_PDHV_PD        0x04
#define PCD8544_LCD_PDHV_H         0x02
#define PCD8544_LCD_PDHV_V         0x01


template<typename rst, typename dc, typename led,
         typename spi_master = SPI0>
class PCD8544 {
public:
  enum { width  = 84 };
  enum { height = 48 };

private:
  /* ------------------------------------------------------------------------ */
  static void writeCmd(uint8_t data) {
    //Tell the LCD that we are writing either to data or a command
    dc::low();
    //Send the data
    spi_master::transferByte(data);
  }

  /* ------------------------------------------------------------------------ */
  static void writeData(uint8_t data) {
    //Tell the LCD that we are writing either to data or a command
    dc::high();

    //Send the data
    spi_master::transferByte(data);
  }

public:
  /* ------------------------------------------------------------------------ */
  static void activate() {
    //Reset the LCD to a known state
    rst::output();
    rst::low();
    _delay_ms(500);
    rst::high();

    //Configure control pins
    dc::output();

    led::output();

    spi_master::activate();
    spi_master::setPrescaler(_spi_clk_4);
    spi_master::setMode(_spi_mode_0);
    spi_master::setBitOrder(_spi_bo_msb);

    //Set Temp coefficient
    // uint8_t temp_coef = 0x00;
    // writeCmd(PCD8544_LCD_TEMP_COEF | (temp_coef & 0x3));

    //LCD bias mode 1:48: Try 0x03 or 0x04
    uint8_t bias = 0x04;
    writeCmd(PCD8544_LCD_BIAS | (bias & 0x07));

    //We must send 0x20 before modifying the display control mode
    writeCmd(PCD8544_LCD_PDHV);

    //Set display control, normal mode. 0x0D for inverse
    writeCmd(PCD8544_LCD_DISP_CTRL | PCD8544_LCD_DISP_CTRL_D);

    setBacklight(0x40);
    setBrightness(50);
  }

  /* ------------------------------------------------------------------------ */
  static void setBacklight(uint8_t power) {
    led::analogWrite(power);
  }

  /* ------------------------------------------------------------------------ */
  static void setBrightness(uint8_t vop) {
    writeCmd(PCD8544_LCD_PDHV | PCD8544_LCD_PDHV_V);
    writeCmd(PCD8544_LCD_VOP | (vop & 0x7F));
    writeCmd(PCD8544_LCD_PDHV);
  }

protected:
  enum partial_update_t { partial_update = false };

  static void display(uint8_t * buffer, uint8_t x_min = 0, uint8_t y_min = 0,
		      uint8_t x_max = height - 1, uint8_t y_max = width - 1) {
    if(partial_update) {
      uint8_t pmax = (y_max / 8 + 1);
      pmax = pmax > 5 ? 5 : pmax;
      for(uint8_t p = (y_min / 8); p < pmax; ++p) {
	writeCmd(PCD8544_LCD_CMD_Y | p);
	writeCmd(PCD8544_LCD_CMD_X | x_min);

	for(uint8_t x = x_min; x <= x_max; ++x) {
	  writeData(buffer[(width*p)+x]);
	}
      }
      writeCmd(PCD8544_LCD_CMD_Y);
    } else {
      writeCmd(PCD8544_LCD_CMD_Y);
      writeCmd(PCD8544_LCD_CMD_X);
      for (int index = 0 ; index < (width * height / 8) ; index++) {
	writeData(buffer[index]);
      }
    }
  }
};


#endif // __PCD8544_HH__
