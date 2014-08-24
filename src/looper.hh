/**
 * @file   looper.hh
 * @author Till Junge <junge@Festwemse-2>
 * @date   Sun Nov  6 23:53:40 2011
 * 
 * @brief  tiny helper class to assure constant loop time
 * 
 * 
 */


#ifndef _LOOPER_H_
#define _LOOPER_H_
#include "WProgram.h"

class Looper
{
public:
  Looper(unsigned int sampling_)
    :sampling(sampling_), 
     micro_sampling(1000*sampling_)
  {}
  void setSampling(unsigned int sampling_)
  {
    this->sampling = sampling_;
  }
  inline void waitNextMicroseconds()
  {
    delayMicroseconds(this->micro_sampling - micros()%this->micro_sampling);
  }
  inline void waitNext()
  {
    delay(this->sampling - millis()%this->sampling);
  }

private:
  unsigned int sampling;
  unsigned int micro_sampling;
};

class MicroLooper
{
public:
  MicroLooper(unsigned int sampling_)
    :sampling(sampling_)
  {}
  void setSampling(unsigned int sampling_)
  {
    this->sampling = sampling_;
  }
 
  inline void waitNext()
  {
    delayMicroseconds(this->sampling - micros()%this->sampling);
  }

private:
  unsigned int sampling;
  unsigned int micro_sampling;
};



#endif /* _LOOPER_H_ */
