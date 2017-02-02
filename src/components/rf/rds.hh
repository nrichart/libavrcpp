#ifndef __RDS_HH__
#define __RDS_HH__

class RDS {
public:
  enum decoded_t {
    _not_decoded = 0x00,
    _pi          = 0x01,
    _prog_name   = 0x02,
    _text        = 0x04,
  };

  enum di_t {
    _dynamic_pty     = 0x08,
    _compressed      = 0x04,
    _artificial_head = 0x02,
    _stereo          = 0x01
  };


  RDS() : decoded(_not_decoded), pi(0), pty(0), di(0), decoding_info(0) {
    memset(this->prog_name, 0,  9);
    memset(this->text     , 0, 65);
  }

  RDS & decode(const uint16_t & rdsa,
	       const uint16_t & rdsb,
	       const uint16_t & rdsc,
	       const uint16_t & rdsd,
	       const uint8_t  & checkwords) {
    if(rdsa != pi) wipe(); // change of station

    this->pi = rdsa;
    this->decoded |= _pi;

    if((checkwords & 0x3) > 2) return *this;

    this->pty = (rdsb >>  5) & 0xF;
    // tp  = (rdsb >> 10) & 0x1;

    uint8_t group_type = (rdsb >> 12) & 0xF;
    uint8_t b0         = (rdsb >> 11) & 0x1;

    if((checkwords >> 4) < 3) {
      if(b0 == 0) { // Version A
	if(((checkwords >> 2) & 0x3) < 3) {
	  switch(group_type) {
	  case 0:
	    decode0B(rdsb & 0x1F, rdsd); // block c contain alternative frequency the we will not decode here
	    break;
	  case 2:
	    decode2A(rdsb & 0x1F, rdsc, rdsd);
	    break;
	  }
	}
      } else {
	switch(group_type) {
	case 0:
	  decode0B(rdsb & 0x1F, rdsb);
	  break;
	case 2:
	  decode2B(rdsb & 0x1F, rdsd);
	  break;
	}
      }
    }

    return *this;
  }

  bool hasPI()       { return this->decoded & _pi; }
  bool hasProgName() { return this->decoded & _prog_name; }
  bool hasText()     { return this->decoded & _text; }

  char * getProgName() { return this->prog_name; }
  char * getText()     { return this->text; }

  void wipeText() {
    memset(this->text     , 0, 64);
    this->decoding_info = 0;
    this->decoded &= ~_text;
  }

  void wipe() {
    wipeText();
    memset(this->prog_name, 0,  8);
    this->di = 0;
    this->decoded = 0;
    this->pi = 0;
    this->pty = 0;
  }

private:

  void decode0B(uint8_t c, const uint16_t & rdsd) {
    decodeDI(c & 0x7);
    c &= 0x3;

    this->prog_name[2*c    ] = rdsd >> 8  ;
    this->prog_name[2*c + 1] = rdsd & 0xFF;

    uint8_t n = (this->decoded >> 4) + 1;

    if(n > 3) decoded |= _prog_name;
    this->decoded = (this->decoded & 0x0F) | ((n << 4) & 0xF0);
  }

  void decode2A(uint8_t c, const uint16_t & rdsc, const uint16_t & rdsd) {
    this->decodeTextAB(c);
    c = c & 0x0F;

    this->text[4*c    ] =  rdsc >> 8  ;
    this->text[4*c + 1] =  rdsc & 0xFF;
    this->text[4*c + 2] =  rdsd >> 8  ;
    this->text[4*c + 3] =  rdsd & 0xFF;

    uint8_t n = (this->decoding_info & 0xEF) + 4;
    if(n > 63) this->decoded |= _text;
    this->decoding_info |= n & 0xEF;
  }

  void decode2B(uint8_t c, const uint16_t & rdsd) {
    this->decodeTextAB(c);
    c = c & 0x0F;

    this->text[2*c    ] =  rdsd >> 8  ;
    this->text[2*c + 1] =  rdsd & 0xFF;

    uint8_t n = (this->decoding_info & 0xEF) + 2;
    if(n > 31) this->decoded |= _text;
    this->decoding_info |= n & 0xEF;
  }

  void decodeTextAB(const uint8_t & c) {
    uint8_t t_ab = c >> 4;
    if(t_ab != (this->decoding_info >> 7)) this->wipeText();
    this->decoding_info |= t_ab << 7;
  }

  void decodeDI(const uint8_t & c) {
    this->di |= (c >> 2) << (3 - (c & 0x3));
  }
private:
  uint8_t decoded;
  uint16_t pi;
  uint8_t pty;
  char prog_name[9];
  char text[65];

  uint8_t di;

  uint8_t decoding_info; // text_ab[7] text[6:0]
};

#endif /* __RDS_HH__ */
