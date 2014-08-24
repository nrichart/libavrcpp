/**
   Created by Øyvind Schei on 15.05.11.
   Copyright 2011 __MyCompanyName__. All rights reserved.
*/

/**
   Modifications:
   - This file as been modified to be integrated in libavrc++ by netWorms
   - Added the missing commands from the datasheet by netWorms
*/


#ifndef SFE_BACKPACK_HH
#define SFE_BACKPACK_HH

template<typename serial = Serial0, uint8_t width_ = 128, uint8_t height_ = 64>
class SFEBackpackLCD {
private:
  enum command_t {
    _sfe_cmd_clear           = 0x00,
    _sfe_cmd_debug           = 0x01,
    _sfe_cmd_backlight       = 0x02,
    _sfe_cmd_circle          = 0x03,
    _sfe_cmd_crlf            = 0x04,
    _sfe_cmd_earse_block     = 0x05,
    _sfe_cmd_reset           = 0x06,
    _sfe_cmd_baudrate        = 0x07,
    _sfe_cmd_toggle_font     = 0x08,
    _sfe_cmd_font_mode       = 0x0A,
    _sfe_cmd_draw_sprite     = 0x0B,
    _sfe_cmd_draw_line       = 0x0C,
    _sfe_cmd_upload_sprite   = 0x0D,
    _sfe_cmd_draw_box        = 0x0F,
    _sfe_cmd_draw_pixel      = 0x10,
    _sfe_cmd_draw_filled_box = 0x12,
    _sfe_cmd_reverse         = 0x14,
    _sfe_cmd_toggle_splash   = 0x15,
    _sfe_cmd_draw_data       = 0x16,
    _sfe_cmd_goto_x          = 0x18,
    _sfe_cmd_goto_y          = 0x19,

  };

  static void sendCommand(command_t c) {
    serial::sendByte(0x7C);
    serial::sendByte(c);
  }

public:
  enum { width  = width_  };
  enum { height = height_ };

  enum draw_mode_t {
    _sfe_font_mode_and  = 0,
    _sfe_font_mode_nand = 1,
    _sfe_font_mode_or   = 2,
    _sfe_font_mode_nor  = 3,
    _sfe_font_mode_xor  = 4,
    _sfe_font_mode_nxor = 5,
    _sfe_font_mode_copy = 7
  };

  enum baudrate_t {
    _sfe_baudrate_4800   = 1,
    _sfe_baudrate_9600   = 2,
    _sfe_baudrate_19200  = 3,
    _sfe_baudrate_38400  = 4,
    _sfe_baudrate_57600  = 5,
    _sfe_baudrate_115200 = 6
  };


  /**
    * Clears the display, eraseBlock is probably faster if you dont need to
    * erase the entire screen.
    **/
  static void clear() {
    sendCommand(_sfe_cmd_clear);
  }

  /**
   * @param[in] level
   * - 0 = no messages
   * - 1 = binary messages, the display will echo a byte = command when the
   *   command is started and a byte = 0x00 when the command is finished
   * - 2 = text messages, useful when using a terminal. will echo
   *   "Entered Command n" when command n is startedand will echo
   *   "Exited Command n, RX_buffer=y" when the command is done, and will report
   *   how many bytes are in the recieve buffer
   **/
  static void setDebug(uint8_t level) {
    sendCommand(_sfe_cmd_debug);
    serial::sendByte(level);
  }

  /**
   * Sets backlight duty cycle.
   * @params[in] duty_cycle 0-100, 0=off.
   */
  static void backlight(uint8_t x) {
    sendCommand(_sfe_cmd_backlight);
    serial::sendByte(x);
  }

  /**
   * Draws a circle at (x,y) with radius r, draw/erase
   */
  static void drawCircle(uint8_t x, uint8_t y, uint8_t r, bool draw) {
    sendCommand(_sfe_cmd_circle);
    serial::sendByte(x);
    serial::sendByte(y);
    serial::sendByte(r);
    serial::sendByte(draw);
  }

  /**
   * Toggles CR/LF. each time this command is sent,
   * wheather or not a CR automatically executes a LF is toggled.
   * this is saved to EEPROM and is persistant over power cycles.
   **/
  static void toggleCRLF() {
    sendCommand(_sfe_cmd_crlf);
  }

  /**
   * Draws a block on the screen with clear or set pixels (depending on if the
   * screen is inverted or not)
   **/
  static void eraseBlock(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    sendCommand(_sfe_cmd_earse_block);
    serial::sendByte(x1);
    serial::sendByte(y1);
    serial::sendByte(x2);
    serial::sendByte(y2);
  }

  /// Resets the LCD, clears the screen and resets x,y offsets to 0,0
  static void reset() {
    sendCommand(_sfe_cmd_reset);
  }

  /**
   * Changes the baudrate.
   * Persistant over power-cycles. Transmitting to the screen during splash
   * resets the baudrate to 115200
   **/
  static void changeBaud(baudrate_t baudrate) {
    sendCommand(_sfe_cmd_baudrate);
    serial::sendByte(baudrate);
  }

  /**
   * Toggles between the standard and AUX font. Only affects text written after
   * the command.
   **/
  static void toggleFont() {
    sendCommand(_sfe_cmd_toggle_font);
  }

  /// Sets the mode for which text is written to the text.
  static void setFontMode(draw_mode_t mode) {
    sendCommand(_sfe_cmd_font_mode);
    serial::sendByte(mode);
  }

  /**
   * Draws a sprite saved in the backpack. x and y sets the upper left corner,
   * n is the number of the stored sprite, n sets the mode (same modes ad text).
   * sprite 0 is the sparkfun logo, other uploaded sprites will be deleted
   * if power is removed.
   **/
  static void drawSprite(uint8_t x, uint8_t y, uint8_t n, draw_mode_t mode) {
    sendCommand(_sfe_cmd_draw_sprite);
    serial::sendByte(x);
    serial::sendByte(y);
    serial::sendByte(n);
    serial::sendByte(mode);
  }

  /// Draws a line from x1,y1 to x2,y2
  static void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool draw) {
    sendCommand(_sfe_cmd_draw_line);
    serial::sendByte(x1);
    serial::sendByte(y1);
    serial::sendByte(x2);
    serial::sendByte(y2);
    serial::sendByte(draw);
  }

  /**
   * Uploads a sprite to the backpack, data must be 32 bytes long.
   * 8 sprites can be stored (0-7).
   * The sparkfun logo is number 0, can be overwritten but reverts when power is removed.
   * All uploaded sprites will be erased upon removal of power
   * Consult with firmware README for more info
   **/
  static void uploadSprite(uint8_t n, uint8_t w, uint8_t h, uint8_t *data) {
    sendCommand(_sfe_cmd_upload_sprite);
    serial::sendByte(n);
    serial::sendByte(w);
    serial::sendByte(h);
    for(uint8_t x = 0; x < 32;x++) serial::sendByte(data[x]);
  }

  /// Draws a rectangle starting from x1,y1 to x2,y2.
  static void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool draw = true) {
    sendCommand(_sfe_cmd_draw_box);
    serial::sendByte(x1);
    serial::sendByte(y1);
    serial::sendByte(x2);
    serial::sendByte(y2);
    serial::sendByte(draw);
  }

  /// Toggles a pixel. x and y for coord.
  static void drawPixel(uint8_t x, uint8_t y, bool draw = true)  {
    sendCommand(_sfe_cmd_draw_pixel);
    serial::sendByte(x % width);
    serial::sendByte(y % height);
    serial::sendByte(draw);
  }

  /**
   * Same as drawBox, but accepts a fill byte. 0xFF for black, 0x00 for white.
   * Other values can be used to create patterns (like 0x55).
   **/
  static void drawFilledBox(uint8_t x1, uint8_t y1,
                            uint8_t x2, uint8_t y2,
                            uint8_t fill) {
    sendCommand(_sfe_cmd_draw_filled_box);
    serial::sendByte(x1);
    serial::sendByte(y1);
    serial::sendByte(x2);
    serial::sendByte(y2);
    serial::sendByte(fill);
  }

  /// Reverses the "color" (black on white / white on black)
  static void reverse() {
    sendCommand(_sfe_cmd_reverse);
  }

  /// Toggles the sparkfun logo during boot.
  static void toggleSplash() {
    sendCommand(_sfe_cmd_toggle_splash);
  }

  /**
   * Allows you to draw graphics to the screen like sprites, but the data
   * doesn't have to be uploaded first, and there is no size restrictions.
   * (other than the drawable area of the display).
   * Requires the length of the data array in addition to the data
   * (found no other way of doing this..)
   * Consult with firmware README for more info
   **/
  static void drawData(uint8_t data[],
                       uint8_t x, uint8_t y,
                       uint8_t w, uint8_t h,
                       draw_mode_t mode = _sfe_font_mode_copy,
                       uint8_t length = 0) {
    sendCommand(_sfe_cmd_draw_data);
    serial::sendByte(x);
    serial::sendByte(y);
    serial::sendByte(mode);
    serial::sendByte(w);
    serial::sendByte(h);
    if(length == 0) length = (w * h) / 8;
    for(x = 0; x < length; x++) serial::sendByte(data[x]);
  }

  /// Sets the x and y offsets, text written after this command will start from x,y.
  static void gotoPosition(uint8_t x, uint8_t y) {
    sendCommand(_sfe_cmd_goto_x);
    serial::sendByte(x);
    sendCommand(_sfe_cmd_goto_y);
    serial::sendByte(y);
  }

  /**
   * Uses the gotoPosition function to select "line" 1-8 on the display.
   * Text can be written between these lines using gotoPosition.
   * This function makes it simpler.
   **/
  static void gotoLine(uint8_t line) {
    uint8_t y;
    if(line > 8) line = 8;
    else if(line < 1) line = 1;
    else
      y = -8 + line * 8;
    gotoPosition(1, y);
  }
};

#endif // SFE_BACKPACK_HH
