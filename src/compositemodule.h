//*********************************************************
//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

#ifndef COMPOSITEMODULE_H
#define COMPOSITEMODULE_H

#include <vector>
#include <math.h>
#include "application.h"
#include "emitter.h"
#include "hsicolor.h"
#include "debug.h"

struct componentEmitter {
  componentEmitter (const Emitter *e, uint16_t la, float a, float s) {
    emitter = e;
    localAddress = la;
    angle = a;
    slope = s;
  }

  componentEmitter() {}

  const Emitter *emitter;
  uint16_t localAddress;
  float angle;
  float slope;
};

class CompositeModule {
  private:
    std::vector<std::shared_ptr<componentEmitter>> _colorEmitters;
    componentEmitter _whiteEmitter;
  public:
    CompositeModule (void);
    void addWhiteEmitter (const Emitter &white, uint16_t localAddress);
    void addColorEmitter(const Emitter &emitter, uint16_t localAddress);
    float getAngle(int emitternum);
    float getSlope(int emitternum);
    std::vector<outputEmitter> Hue2EmitterPower(const HSIColor &HSI) const;
};

#endif
