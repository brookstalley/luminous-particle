//*********************************************************
//
// Luminous
// Copyright 2017 Brooks Talley
//
// Portions derived from TeensyLED, copyright 2015 Brian Neltner
//
// Luminous is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Luminous is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//
//**********************************************************

#include "hsilight.h"

#include "luminous-particle.h"

#include "hsicolor.h"
#include "compositemodule.h"
#include "outputinterface.h"
#include "temperatureinterface.h"
#include "ldebug.h"

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
{
}

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
{
}

void HSILight::begin() {
	_emitterPowers = _compositeModule.emitterPowersFromHSI(HSIColor(0, 0, 0));
	_lastE131PacketCount = 0;
	debugPrintf(DEBUG_TRACE, "HSILight: begin (%u)", _emitterPowers.size());

	for (unsigned int i = 0; i < _emitterPowers.size(); i++) {
		debugPrintf(DEBUG_INSANE, "HSILight: emitter %u la %u", i, _emitterPowers[i].outputLocalAddress);
	}
}

void HSILight::setColor(const HSIColor& color)  {
	_emitterPowers = _compositeModule.emitterPowersFromHSI(color);
	/*
	      for (unsigned int i = 0; i < _emitterPowers.size(); i++) {
	              debugPrintf(DEBUG_INSANE, "HSILight: emitter %u power %f", i, _emitterPowers[i].power);
	      }
	 */
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
	_outputInterface->setEmitterPowers(_emitterPowers, scaleFactor);
}

const char * HSILight::getName(void) const {
	return _name;
}

std::shared_ptr<E131> HSILight::getE131() const {
	return _e131;
}

const uint16_t HSILight::getE131LocalAddress() const {
	return _e131LocalAddress;
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
