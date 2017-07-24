#include "hsilamp.h"

HSILamp::HSILamp(const CompositeLight &compositeLight, const PCA9685 &pwm,
  const uint8_t pwmAddress) :
    _compositeLight(compositeLight),
    _pwm(pwm),
    _pwmAddress(pwmAddress)
{

}

void HSILamp::begin() {
  std::vector<outputEmitter> powers;
  powers = _compositeLight.Hue2EmitterPower(HSIColor(0,0,0));
  _emitterPowers = _compositeLight.Hue2EmitterPower(HSIColor(0,0,0));
}

void HSILamp::setColor(const HSIColor &color) {
  _emitterPowers = _compositeLight.Hue2EmitterPower(color);
  this->setEmitters();
}

void HSILamp::setSingleEmitterOn(unsigned int index) {
  //char msg[100];
  //sprintf(msg,"Setting single emitter for index %u",index);
  //debugPrint(msg);
  for (unsigned int i=0; i < _emitterPowers.size(); i++) {
    _emitterPowers[i].power = (index % _emitterPowers.size() == i) ? 1.0 : 0.0;
  }
  this->setEmitters();
}

void HSILamp::setEmitters() {
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
