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
  _localBrightness(1.0f),
  _temperature(-1.0f)
{}

// Constructor with no temperature sensor; set the interface to nullptr,
// and adddress to 0
HSILight::HSILight(const char *name, const CompositeModule& compositeModule,
                   OutputInterface& outputInterface, const uint16_t outputLocalAddress
                   ) :
  _name(name),
  _compositeModule(compositeModule),
  _outputInterface(outputInterface),
  _outputLocalAddress(outputLocalAddress),
  _temperatureInterface(nullptr),
  _temperatureLocalAddress(0),
  _localBrightness(1.0f),
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
  debugPrint(DEBUG_INSANE, "HSILight: setting emitters");
  float scaleFactor = globalBrightness * _localBrightness;
  _outputInterface.setEmitterPowers(_emitterPowers, scaleFactor);
}

const char * HSILight::getName(void) const {
  return _name;
}

float HSILight::updateTemperature(void) {
  if (_temperatureInterface == nullptr) return -1.0f;
  _temperature = _temperatureInterface->getTemperature();
  return _temperature;
}

float HSILight::getTemperature(void) const {
  if (_temperatureInterface != nullptr) {
    if ((millis() - _tempertureUpdatedMillis) < TEMPERATURE_VALID_MILLIS) {
      return _temperature;
    } else {
      return updateTemperature();
    }
  } else {
    return -1.0f;
  }
}
