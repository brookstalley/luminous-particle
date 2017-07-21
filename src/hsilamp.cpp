#include "hsilamp.h"

HSILamp::HSILamp(Colorspace &colorspace, byte i2cAddress,
  byte pwmAddress) :
    _colorspace(colorspace),
    _i2caddress(i2cAddress),
    _pwmAddress(pwmAddress),
{
  HSIColor off(0,0,0);
  _emitterPowers = off;
}

void HSILamp::setColor(HSIColor &color) {
  _emitterPowers = _compositeLight->Hue2EmitterPower(color);
  setEmitters();
}

void HSILamp::setSingleEmitterOn(unsigned int index) {
  for (int i=0; i < _emitterPowers.size(); i++) {
    _emitterPowers[i].power = (index % _emitterPowers.size() == 0) ? 1.0 : 0.0;
  }
  setEmitters();
}

void HSILamp::setEmitters() {
  for (int i=0; i<emitters.size(); i++) {
    sendI2CPwm(i2cAddress, pwmAddress + _emitterPowers[i].pwmOffset,
      0xFFFF * _emitterPowers[i].power * globalBrightness)
  }
}
