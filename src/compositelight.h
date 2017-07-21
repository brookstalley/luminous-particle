//*********************************************************
//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

#ifndef COMPOSITELIGHT_H
#define COMPOSITELIGHT_H

#include <vector>
#include "application.h"
#include "emitter.h"
#include "hsicolor.h"


struct outputEmitter {
  Emitter &emitter;
  // within a light of this type, what
  uint8_t pwmOffset;
  float power;
};

class CompositeLight {
  private:
    std::vector<outputEmitter> _colorEmitters;
    outputEmitter _white;
    std::vector<float> _slope;
    std::vector<float> _angle;
  public:
    CompositeLight (Emitter &white, uint8_t pwmOffset);
    void addEmitter(Emitter &emitter, uint8_t pwmOffset);
    float getAngle(int emitternum);
    float getSlope(int emitternum);
    std::vector<outputEmitter> Hue2EmitterPower(HSIColor &HSI);
};

#endif
