/**
  This is a library for the BMP085 pressure sensor

  Designed specifically to work with the Adafruit BMP085 or BMP180 Breakout
  ----> http://www.adafruit.com/products/391
  ----> http://www.adafruit.com/products/1603

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
*/

/**
   Modifications:
   - 08/2014 this file as been integrated in libavrc++ by netWorms
 */



#ifndef __BMP085_H__
#define __BMP085_H__

#include "twi.hh"
//#define BMP085_TEST_DATA

template<class twi = TWI0<0x77>>
class BMP085 {
public:
  enum mode_t {
    _ultralowpower     = 0,
    _standard          = 1,
    _highres           = 2,
    _ultrahighres      = 3
  };

private:
  enum reg_t {
    _readtempcmd     = 0x2E,
    _readpressurecmd = 0x34,
    _cal_ac1         = 0xAA,
    _cal_ac2         = 0xAC,
    _cal_ac3         = 0xAE,
    _cal_ac4         = 0xB0,
    _cal_ac5         = 0xB2,
    _cal_ac6         = 0xB4,
    _cal_b1          = 0xB6,
    _cal_b2          = 0xB8,
    _cal_mb          = 0xBA,
    _cal_mc          = 0xBC,
    _cal_md          = 0xBE,
    _chipid          = 0xD0,
    _version         = 0xD1,
    _softreset       = 0xE0,
    _control         = 0xF4,
    _tempdata        = 0xF6,
    _pressuredata    = 0xF6
  };

  static inline int32_t getB5(int32_t UT) {
    int32_t X1 = ((UT - (int32_t)calibration_data.AC6) * (int32_t)calibration_data.AC5) >> 15;
    int32_t X2 = ((int32_t)calibration_data.MC << 11) / (X1 + (int32_t)calibration_data.MD);
    int32_t B5 = X1 + X2;

#if defined(BMP085_TEST_DATA)
    Serial0::printf("X1=%10ld\n\r", X1);
    Serial0::printf("X2=%10ld\n\r", X2);
    Serial0::printf("B5=%10ld\n\r", B5);
#endif
    return B5;
  }

public:
  static void activate(mode_t _mode = _standard) {
    twi::activate();

    mode = _mode;
    uint8_t id;
    twi::registerRead(_chipid, id);

    readCalibration();
  }

  static int32_t getTemperature() {
    int32_t UT = readRawTemperature();
    int32_t B5 = getB5(UT);
    int32_t temp = ((B5 + 8) >> 4);

#if defined(BMP085_TEST_DATA)
    Serial0::printf("T =%10ld\n\r", temp);
#endif
    return temp;
  }

  static int32_t getPressure() {
    int32_t UT = readRawTemperature();
    int32_t UP = readRawPressure();
    int32_t B5 = getB5(UT);

    int32_t  B6 = B5 - 4000;
    int32_t  X1 = ((int32_t)calibration_data.B2 * (B6 * B6) >> 12) >> 11;
    int32_t  X2 = ((int32_t)calibration_data.AC2 * B6) >> 11;
    int32_t  X3 = X1 + X2;
    int32_t  B3 = ((((int32_t)calibration_data.AC1 * 4 + X3) << mode) + 2) / 4;
#if defined(BMP085_TEST_DATA)
    Serial0::printf("B6=%10ld\n\r", B6);
    Serial0::printf("X1=%10ld\n\r", X1);
    Serial0::printf("X2=%10ld\n\r", X2);
    Serial0::printf("X3=%10ld\n\r", X3);
    Serial0::printf("B3=%10ld\n\r", B3);
#endif

             X1 = ((int32_t)calibration_data.AC3 * B6) >> 13;
             X2 = ((int32_t)calibration_data.B1 * ((B6 * B6) >> 12)) >> 16;
             X3 = ((X1 + X2) + 2) >> 2;
    uint32_t B4 = ((int32_t)calibration_data.AC4 * (uint32_t)(X3 + 32768)) >> 15;
    uint32_t B7 = (uint32_t) (UP - B3) * (50000 >> mode);
    int32_t p;

#if defined(BMP085_TEST_DATA)
    Serial0::printf("X1=%10ld\n\r", X1);
    Serial0::printf("X2=%10ld\n\r", X2);
    Serial0::printf("X3=%10ld\n\r", X3);
    Serial0::printf("B4=%10ld\n\r", B4);
    Serial0::printf("B7=%10ld\n\r", B7);
#endif

    if (B7 < 0x80000000) p = (B7 * 2) / B4;
    else                 p = (B7 / B4) * 2;

    X1 = (p >> 8) * (p >> 8);

#if defined(BMP085_TEST_DATA)
    Serial0::printf("p =%10ld\n\r", p);
    Serial0::printf("X1=%10ld\n\r", X1);
#endif

    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;

    p = p + ((X1 + X2 + 3791) >> 4);

#if defined(BMP085_TEST_DATA)
    Serial0::printf("X1=%10ld\n\r", X1);
    Serial0::printf("X2=%10ld\n\r", X2);
    Serial0::printf("p =%10ld\n\r", p);
#endif

    return p;
  }

private:
  static uint16_t readRawTemperature() {
#if defined(BMP085_TEST_DATA)
    return 27898;
#else
    twi::registerWrite(uint8_t(_control), uint8_t(_readtempcmd));
    _delay_ms(5);
    uint16_t temp;
    twi::registerRead (_tempdata, temp);
    return temp;
#endif
  }

  static int32_t readRawPressure() {
#if defined(BMP085_TEST_DATA)
    return 23843;
#else
    twi::registerWrite(_control, uint8_t(_readpressurecmd | mode << 6));

    switch(mode) {
    case _ultralowpower: _delay_ms(5) ; break;
    case _standard     : _delay_ms(7) ; break;
    case _highres      : _delay_ms(14); break;
    case _ultrahighres : _delay_ms(26); break;
    }

    uint16_t p; uint8_t xp;
    twi::registerRead(_pressuredata    , p );
    twi::registerRead(_pressuredata + 2, xp);
    int32_t pressure = ((uint32_t(p) << 8) + xp) >> (8 - mode);
    return pressure;
#endif
  }


  static void readCalibration() {
#if defined(BMP085_TEST_DATA)
    calibration_data.AC1 =    408;
    calibration_data.AC2 =    -72;
    calibration_data.AC3 = -14383;
    calibration_data.AC4 =  32741;
    calibration_data.AC5 =  32757;
    calibration_data.AC6 =  23153;
    calibration_data.B1  =   6190;
    calibration_data.B2  =      4;
    calibration_data.MB  = -32768;
    calibration_data.MC  =  -8711;
    calibration_data.MD  =   2868;
    mode = 0;

    Serial0::printf("AC1=%10d\n\r", calibration_data.AC1);
    Serial0::printf("AC2=%10d\n\r", calibration_data.AC2);
    Serial0::printf("AC3=%10d\n\r", calibration_data.AC3);
    Serial0::printf("AC4=%10d\n\r", calibration_data.AC4);
    Serial0::printf("AC5=%10d\n\r", calibration_data.AC5);
    Serial0::printf("AC6=%10d\n\r", calibration_data.AC6);
    Serial0::printf("B1 =%10d\n\r", calibration_data.B1 );
    Serial0::printf("B2 =%10d\n\r", calibration_data.B2 );
    Serial0::printf("MB =%10d\n\r", calibration_data.MB );
    Serial0::printf("MC =%10d\n\r", calibration_data.MC );
    Serial0::printf("MD =%10d\n\r", calibration_data.MD );


#else
    twi::registerRead(_cal_ac1, calibration_data.AC1);
    twi::registerRead(_cal_ac2, calibration_data.AC2);
    twi::registerRead(_cal_ac3, calibration_data.AC3);
    twi::registerRead(_cal_ac4, calibration_data.AC4);
    twi::registerRead(_cal_ac5, calibration_data.AC5);
    twi::registerRead(_cal_ac6, calibration_data.AC6);
    twi::registerRead(_cal_b1 , calibration_data.B1 );
    twi::registerRead(_cal_b2 , calibration_data.B2 );
    twi::registerRead(_cal_mb , calibration_data.MB );
    twi::registerRead(_cal_mc , calibration_data.MC );
    twi::registerRead(_cal_md , calibration_data.MD );
#endif
  }

private:
  struct CalibrationData {
    int16_t  AC1;
    int16_t  AC2;
    int16_t  AC3;
    uint16_t AC4;
    uint16_t AC5;
    uint16_t AC6;
    int16_t  B1;
    int16_t  B2;
    int16_t  MB;
    int16_t  MC;
    int16_t  MD;
  };

  static CalibrationData calibration_data;
  static uint8_t mode;
};

template< class twi>
typename BMP085<twi>::CalibrationData BMP085<twi>::calibration_data;

template< class twi>
uint8_t BMP085<twi>::mode;

#endif /* __BMP085_H__ */
