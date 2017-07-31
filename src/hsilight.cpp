#include "hsilight.h"

#include "hsicolor.h"
#include "compositemodule.h"
#include "outputinterface.h"
#include "debug.h"

HSILight::HSILight(const char *name, const CompositeModule& compositeModule,
                   OutputInterface& outputInterface, const uint16_t outputLocalAddress,
                   TemperatureInterface& temperatureInterface, const uint16_t temperatureLocalAddress) :
  _name(name),
  _compositeModule(compositeModule),
  _outputInterface(outputInterface),
  _outputLocalAddress(outputLocalAddress),
  _temperatureInterface(temperatureInterface),
  _temperatureLocalAddress(temperatureLocalAddress),
  _temperature(-1.0f)
{}

void HSILight::begin() {
  _emitterPowers = _compositeModule.Hue2EmitterPower(HSIColor(0, 0, 0));
  debugPrintf(DEBUG_TRACE, "HSILight: begin (%u)", _emitterPowers.size());

  for (unsigned int i = 0; i < _emitterPowers.size(); i++) {
    debugPrintf(DEBUG_INSANE, "HSILight: emitter %u la %u", i, _emitterPowers[i].outputLocalAddress);
  }
}

void HSILight::setColor(const HSIColor& color)  {
  _emitterPowers = _compositeModule.Hue2EmitterPower(color);
  setEmitters();
}

void HSILight::setSingleEmitterOn(unsigned int index) {
  debugPrintf(DEBUG_INSANE, "HSILight: Setting single emitter for index %u (%u)", index, _emitterPowers.size());

  float thisEmitter = 0.0f;

  for (unsigned int i = 0; i < _emitterPowers.size(); i++) {
    thisEmitter = (index % _emitterPowers.size() == i) ? 1.0 : 0.0;
    debugPrintf(DEBUG_INSANE, "HSILight: emitter %u power %f", i, thisEmitter);

    _emitterPowers[i].power = thisEmitter;
  }
  setEmitters();
}

void HSILight::setEmitters() {
  debugPrint(DEBUG_INSANE, "HSILight: setting emitters");;
  _outputInterface.setEmitterPowers(_emitterPowers);
}

const char * HSILight::getName(void) const {
  return _name;
}

float HSILight::getTemperature(void) const {
  return _temperature;
}
