#include "hsilight.h"

HSILight::HSILight(const CompositeModule &compositeModule, const PCA9685 &pwm,
  const uint8_t pwmAddress) :
    _compositeModule(compositeModule),
    _pwm(pwm),
    _pwmAddress(pwmAddress)
{

}

void HSILight::begin() {
  std::vector<outputEmitter> powers;
  powers = _compositeModule.Hue2EmitterPower(HSIColor(0,0,0));
  _emitterPowers = _compositeModule.Hue2EmitterPower(HSIColor(0,0,0));
}

void HSILight::setColor(const HSIColor &color) {
  _emitterPowers = _compositeModule.Hue2EmitterPower(color);
  this->setEmitters();
}

void HSILight::setSingleEmitterOn(unsigned int index) {
  //char msg[100];
  //sprintf(msg,"Setting single emitter for index %u",index);
  //debugPrint(msg);
  for (unsigned int i=0; i < _emitterPowers.size(); i++) {
    _emitterPowers[i].power = (index % _emitterPowers.size() == i) ? 1.0 : 0.0;
  }
  this->setEmitters();
}

void HSILight::setEmitters() {
  //debugPrint("setting emitters");
  char msg[100];
  float emitterPower = 0.0f;
  unsigned int pwmChannel;
  uint16_t channelPower = 0;
  for (unsigned int i=0; i < _emitterPowers.size(); i++) {
    emitterPower = _emitterPowers[i].power * globalBrightness;
    channelPower = 0x0FFF * emitterPower;
    pwmChannel = _pwmAddress + _emitterPowers[i].pwmOffset;
    sprintf(msg,"Setting PWM channel %u to %u",pwmChannel, channelPower);
    debugPrint(msg);
    pwm.setChannelPWM(pwmChannel, channelPower);
  }
}
