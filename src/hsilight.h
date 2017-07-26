#ifndef HSILIGHT_H
#define HSILIGHT_H

#include "hsicolor.h"
#include "compositemodule.h"
#include "PCA9685-Particle/PCA9685.h"
#include "debug.h"

extern float globalBrightness;
extern PCA9685 pwm;

class HSILight {
  private:
    const CompositeModule &_compositeModule;
    const OutputInterface &_interface;
    uint16_t _localAddress;

    std::vector<outputEmitter> _emitterPowers;
    void setEmitters();

  public:
    HSILight(const CompositeModule &compositemodule, const OutputInterface &interface, const uint8_t pwmAddress);
    void begin();
    void setColor(const HSIColor &color);
    void getColor(const HSIColor*);
    void setSingleEmitterOn(unsigned int index);
};
#endif
