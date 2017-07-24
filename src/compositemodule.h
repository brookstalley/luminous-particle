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
#include "application.h"
#include "emitter.h"
#include "hsicolor.h"
#include "debug.h"

struct componentEmitter {
  componentEmitter (const Emitter *e, uint8_t pwm, float a, float s) {
    emitter = e;
    pwmOffset = pwm;
    angle = a;
    slope = s;
  }
  const Emitter *emitter;
  uint8_t pwmOffset;
  float angle;
  float slope;
};

struct outputEmitter {
  outputEmitter(uint8_t po, float pwr) {
    pwmOffset = po;
    power = pwr;
  }
  uint8_t pwmOffset;
  float power;
};

class CompositeModule {
  private:
    std::vector<componentEmitter> _colorEmitters;
    componentEmitter _whiteEmitter;
  public:
    CompositeModule (Emitter &white, uint8_t pwmOffset);
    void addEmitter(Emitter &emitter, uint8_t pwmOffset);
    float getAngle(int emitternum);
    float getSlope(int emitternum);
    std::vector<outputEmitter> Hue2EmitterPower(const HSIColor &HSI) const;
};

#endif
