// *********************************************************

//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

#ifndef EMITTER_H
#define EMITTER_H

#include "Particle.h"

struct outputEmitter {
  outputEmitter(uint8_t la, float pwr) {
    localAddress = la;
    power        = pwr;
  }
  uint16_t localAddress;
  float    power;
};

class Emitter {
private:

  float _u, _v, _maxvalue;
  const char *_name;

public:

  Emitter(const char *name, float u, float v, float maxvalue);
  float       getU(void) const;
  float       getV(void) const;
  float       getMax(void) const;
  const char* getName(void) const;
};
#endif /* ifndef EMITTER_H */
