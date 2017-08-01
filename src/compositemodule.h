// *********************************************************

//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

#ifndef COMPOSITEMODULE_H
#define COMPOSITEMODULE_H

#include "emitter.h"
#include "hsicolor.h"

#include "Particle.h"

#include <vector>

struct componentEmitter {
  componentEmitter(const Emitter * e, uint16_t la, float a, float s) {
    emitter            = e;
    outputLocalAddress = la;
    angle              = a;
    slope              = s;
  }

  componentEmitter() {}

  const Emitter *emitter;
  uint16_t       outputLocalAddress;
  float          angle;
  float          slope;
};

class CompositeModule {
private:

  std::vector < std::shared_ptr < componentEmitter >> _colorEmitters;
  componentEmitter _whiteEmitter;
  float _dimTemperature;
  float _shutdownTemperature;

public:

  CompositeModule(float dimTemperature, float shutdownTemperature);
  void  addWhiteEmitter(const Emitter& white,
                        uint16_t outputLocalAddress);
  void  addColorEmitter(const Emitter& emitter,
                        uint16_t outputLocalAddress);
  float getAngle(int emitternum);
  float getSlope(int emitternum);
  std::vector < outputEmitter > Hue2EmitterPower(const HSIColor &HSI) const;
};

#endif /* ifndef COMPOSITEMODULE_H */
