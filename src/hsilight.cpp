#include "hsilight.h"

HSILight::HSILight(const CompositeModule &compositeModule, OutputInterface &interface,
  const uint16_t localAddress) :
    _compositeModule(compositeModule),
    _interface(interface),
    _localAddress(localAddress)
{

}

void HSILight::begin() {
  debugOutput();
  _emitterPowers = _compositeModule.Hue2EmitterPower(HSIColor(0,0,0));
  char msg[50];
  sprintf(msg, "HSILight: begin (%u)", _emitterPowers.size());
  debugPrint(DEBUG_TRACE, msg);

  for (unsigned int i=0; i < _emitterPowers.size(); i++) {
    sprintf(msg,"HSILight: emitter %u la %u", i, _emitterPowers[i].localAddress);
    debugPrint(DEBUG_TRACE, msg);
  }

  debugOutput();
}

void HSILight::setColor(const HSIColor &color)  {
  _emitterPowers = _compositeModule.Hue2EmitterPower(color);
  setEmitters();
}

void HSILight::setSingleEmitterOn(unsigned int index) {
  debugOutput();
  char msg[100];
  sprintf(msg,"HSILight: Setting single emitter for index %u (%u)", index, _emitterPowers.size() );
  debugPrint(DEBUG_TRACE, msg);
  float thisEmitter = 0.0f;
  for (unsigned int i=0; i < _emitterPowers.size(); i++) {

    thisEmitter = (index % _emitterPowers.size() == i) ? 1.0 : 0.0;
    sprintf(msg,"HSILight: emitter %u power %f", i, thisEmitter);
    debugPrint(DEBUG_TRACE, msg);

    _emitterPowers[i].power = thisEmitter;
  }
  setEmitters();
  debugOutput();
}

void HSILight::setEmitters() {
  debugPrint(DEBUG_TRACE, "HSILight: setting emitters");;
  _interface.setEmitterPowers(_emitterPowers);
}

void HSILight::debugOutput() const {
  char msg[100];
  sprintf(msg, "HSILight: _emitterPowers.size() = %u", _emitterPowers.size());
  debugPrint(DEBUG_TRACE, msg);


}
