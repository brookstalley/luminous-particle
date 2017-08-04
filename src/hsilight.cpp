#include "hsilight.h"

#include "hsicolor.h"
#include "compositemodule.h"
#include "outputinterface.h"
#include "temperatureinterface.h"
#include "debug.h"

// Constructor with no temperature sensor; set the interface to nullptr,
// and adddress to 0
HSILight::HSILight(const char *name, const CompositeModule& compositeModule,
                   std::shared_ptr<OutputInterface>outputInterface, const uint16_t outputLocalAddress,
                   const std::shared_ptr<E131>e131, const uint16_t e131LocalAddress,
                   std::shared_ptr<TemperatureInterface>temperatureInterface,
                   const uint16_t temperatureLocalAddress
                   ) :
  _name(name),
  _compositeModule(compositeModule),
  _outputInterface(outputInterface),
  _outputLocalAddress(outputLocalAddress),
  _e131(e131),
  _e131LocalAddress(e131LocalAddress),
  _temperatureInterface(temperatureInterface),
  _temperatureLocalAddress(temperatureLocalAddress),
  _localBrightness(1.0f),
  _temperature(-1.0f)
{}

// Constructor with no temperature sensor; set the interface to nullptr,
// and adddress to 0
HSILight::HSILight(const char *name, const CompositeModule& compositeModule,
                   std::shared_ptr<OutputInterface>outputInterface, const uint16_t outputLocalAddress,
                   const std::shared_ptr<E131>e131, const uint16_t e131LocalAddress
                   ) :
  _name(name),
  _compositeModule(compositeModule),
  _outputInterface(outputInterface),
  _outputLocalAddress(outputLocalAddress),
  _e131(e131),
  _e131LocalAddress(e131LocalAddress),
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

void HSILight::setColorFromE131() {
  HSIColor color(
    twoBytesToFloat(&_e131->data[_e131LocalAddress]),
    twoBytesToFloat(&_e131->data[_e131LocalAddress + 2]),
    twoBytesToFloat(&_e131->data[_e131LocalAddress + 4])
    );

  setColor(color);
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
  _outputInterface->setEmitterPowers(_emitterPowers, scaleFactor);
}

const char * HSILight::getName(void) const {
  return _name;
}

void HSILight::setTemperatureInterface(std::shared_ptr<TemperatureInterface>temperatureInterface) {
  _temperatureInterface = temperatureInterface;
}

float HSILight::updateTemperature(void) {
  if (_temperatureInterface == NULL) return -1.0f;
  _temperature = _temperatureInterface->getTemperature(_temperatureLocalAddress);
  return _temperature;
}

float HSILight::getTemperature(void) {
  if (_temperatureInterface != NULL) {
    if ((millis() - _tempertureUpdatedMillis) < TEMPERATURE_VALID_MILLIS) {
      return _temperature;
    } else {
      return updateTemperature();
    }
  } else {
    return -1.0f;
  }
}

float twoBytesToFloat(uint8_t *buf) {
  return (float)(buf[0] | buf[1] << 8) / 65535.0f;
}
