#include "hsilight.h"

HSILight::HSILight(const CompositeModule &compositeModule, const OutputInterface &interface,
  const uint16_t localAddress) :
    _compositeModule(compositeModule),
    _interface(interface),
    _localAddress(localAddress)
{

}

void HSILight::begin() {
  std::vector<outputEmitter> powers;
  powers = _compositeModule.Hue2EmitterPower(HSIColor(0,0,0));
  _emitterPowers = _compositeModule.Hue2EmitterPower(HSIColor(0,0,0));
}

void HSILight::setColor(const HSIColor &color)  {
  _emitterPowers = _compositeModule.Hue2EmitterPower(color);
  setEmitters();
}

void HSILight::setSingleEmitterOn(unsigned int index) {
  //char msg[100];
  //sprintf(msg,"Setting single emitter for index %u",index);
  //debugPrint(msg);
  for (unsigned int i=0; i < _emitterPowers.size(); i++) {
    _emitterPowers[i].power = (index % _emitterPowers.size() == i) ? 1.0 : 0.0;
  }
  setEmitters();
}

void HSILight::setEmitters() {
  //debugPrint("setting emitters");
    _interface.setEmitterPowers(_emitterPowers);
}
