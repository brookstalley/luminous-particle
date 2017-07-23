#ifndef HSILAMP_H
#define HSILAMP_H

#include "hsicolor.h"
#include "compositelight.h"
#include "PCA9685-Particle/PCA9685.h"
#include "debug.h"

extern float globalBrightness;
extern PCA9685 pwm;

class HSILamp {
  private:
    const CompositeLight &_compositeLight;
    const PCA9685 &_pwm;
    uint8_t _pwmAddress;
    std::vector<outputEmitter> _emitterPowers;
    void setEmitters();

  public:
    HSILamp(const CompositeLight &compositelight, const PCA9685 &pwm, const uint8_t pwmAddress);
    void begin();
    void setColor(const HSIColor &color);
    void getColor(const HSIColor*);
    void setSingleEmitterOn(unsigned int index);
};
#endif
