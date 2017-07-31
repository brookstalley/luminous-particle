#include "hsilight.h"
#include "hsicolor.h"
#include "compositemodule.h"
#include "outputinterface.h"
#include "debug.h"

HSILight::HSILight(const char *name, const CompositeModule& compositeModule, OutputInterface& interface,
                   const uint16_t localAddress) :
  _name(name),
  _compositeModule(compositeModule),
  _interface(interface),
  _localAddress(localAddress)
{}

void HSILight::begin() {
  _emitterPowers = _compositeModule.Hue2EmitterPower(HSIColor(0, 0, 0));
  char msg[50];
  sprintf(msg, "HSILight: begin (%u)", _emitterPowers.size());
  debugPrint(DEBUG_TRACE, msg);

  for (unsigned int i = 0; i < _emitterPowers.size(); i++) {
    debugPrintf(DEBUG_INSANE, "HSILight: emitter %u la %u", i, _emitterPowers[i].localAddress);
  }
}

void HSILight::setColor(const HSIColor& color)  {
  _emitterPowers = _compositeModule.Hue2EmitterPower(color);
  setEmitters();
}

void HSILight::setSingleEmitterOn(unsigned int index) {
  debugPrintf(DEBUG_TRACE, "HSILight: Setting single emitter for index %u (%u)", index, _emitterPowers.size());

  float thisEmitter = 0.0f;

  for (unsigned int i = 0; i < _emitterPowers.size(); i++) {
    thisEmitter = (index % _emitterPowers.size() == i) ? 1.0 : 0.0;
    debugPrintf(DEBUG_INSANE, "HSILight: emitter %u power %f", i, thisEmitter);

    _emitterPowers[i].power = thisEmitter;
  }
  setEmitters();
}

void HSILight::setEmitters() {
  debugPrint(DEBUG_TRACE, "HSILight: setting emitters");;
  _interface.setEmitterPowers(_emitterPowers);
}

const char * HSILight::getName(void) const {
  return _name;
}
