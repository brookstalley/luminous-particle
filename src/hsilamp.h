#ifndef HSILAMP_H
#define HSILAMP_H

#include "hsicolor.h"
#include "compositelight.h"
#include "Adafruit_PWMServoDriver.h"

extern float globalBrightness;
extern Adafruit_PWMServoDriver pwm;

class HSILamp {
  private:
    CompositeLight &_compositeLight;
    uint8_t _i2cAddress;
    uint8_t _pwmAddress;
    std::vector<outputEmitter> _emitterPowers;
    void setEmitters();

  public:
    HSILamp(CompositeLight &compositelight, uint8_t i2caddress, uint8_t pwmAddress);
    void setColor(HSIColor &color);
    void getColor(HSIColor*);
    void setSingleEmitterOn(unsigned int index);
};
#endif
