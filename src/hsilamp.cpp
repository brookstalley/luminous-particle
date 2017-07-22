#include "hsilamp.h"

HSILamp::HSILamp(CompositeLight &compositeLight, uint8_t i2cAddress,
  uint8_t pwmAddress) :
    _compositeLight(compositeLight),
    _i2cAddress(i2cAddress),
    _pwmAddress(pwmAddress)
{
  HSIColor black = HSIColor(0,0,0);
  this->setColor (black);
}

void HSILamp::setColor(HSIColor &color) {
  _emitterPowers = _compositeLight.Hue2EmitterPower(color);
  this->setEmitters();
}

void HSILamp::setSingleEmitterOn(unsigned int index) {
  for (int i=0; i < _emitterPowers.size(); i++) {
    _emitterPowers[i].power = (index % _emitterPowers.size() == 0) ? 1.0 : 0.0;
  }
  this->setEmitters();
}

void HSILamp::setEmitters() {
  for (int i=0; i < _emitterPowers.size(); i++) {
    pwm.setPin(_pwmAddress + _emitterPowers[i].pwmOffset,
      0x0FFF * _emitterPowers[i].power * globalBrightness, false);
  }
}
