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

// CompositeModule models a light that contains multiple emitters.
//
// After constructing, add white emitter using addWhiteEmitter and
// add color emitters using addColorEmitter.

#include "compositemodule.h"

#include "emitter.h"
#include "hsicolor.h"
#include "ldebug.h"

#include "Particle.h"

#include <vector>
#include <math.h>

CompositeModule::CompositeModule(float dimTemperature, float shutdownTemperature) :
	_dimTemperature(dimTemperature),
	_shutdownTemperature(shutdownTemperature)
{
}

void CompositeModule::addWhiteEmitter(const Emitter& white, uint16_t outputLocalAddress) {
	// slope and angle not relevant for white emitter
	// currently supports only one white emitter per composite module
	_whiteEmitter = componentEmitter(&white, outputLocalAddress, 0.0f, 0.0f);
	debugPrintf(DEBUG_INSANE, "Added white emitter %s at la %u", white.getName(), outputLocalAddress);
}

void CompositeModule::addColorEmitter(const Emitter& emitter, uint16_t outputLocalAddress) {
	// To figure out where to put it in the colorspace, calculate the angle from
	// the white point.
	float uEmitter = emitter.getU();
	float vEmitter = emitter.getV();
	float uWHITE   = _whiteEmitter.emitter->getU();
	float vWHITE   = _whiteEmitter.emitter->getV();

	debugPrintf(DEBUG_TRACE, "CompositeModule::addColorEmitter %s at address %u", emitter.getName(), outputLocalAddress);

	float newAngle = fmod((180 / M_PI) * atan2((vEmitter - vWHITE), (uEmitter - uWHITE)) + 360, 360);

	if (_colorEmitters.empty()) {
		// If it is the first LED, simply place it in the array.
		// With only one LED, slope is undefined.
		_colorEmitters.push_back(std::make_shared<componentEmitter>(componentEmitter(&emitter, outputLocalAddress, newAngle,
		                                                                             0)));
		debugPrintf(DEBUG_INSANE, "Added emitter %s at la %u", emitter.getName(), outputLocalAddress);
		return;
	}

	// Otherwise, place the LED at the appropriate point in the array, and also
	// recalculate slopes.

	// Create our component emitter based on the prototype emitter, plus this
	// one's particular address
	// TODO: This doesn't need to be a shared pointer. Only this class will ever
	// use it.
	std::shared_ptr<componentEmitter> newEmitter =
		std::make_shared<componentEmitter>(componentEmitter(&emitter, outputLocalAddress, newAngle, 0));

	// Iterate through until finding the first location where the angle is bigger
	// than the current value.
	// TODO: Fix to use iterator
	std::vector<std::shared_ptr<componentEmitter> >::iterator it = _colorEmitters.begin();

	while ((it < _colorEmitters.end()) && ((*it)->angle < newAngle)) {
		it++;
	}

	// We will now either be in the correct slot in the vector, or at the end of
	// the vector
	// Insert the new emitter, set the pwm offset, set slope to zero pending
	// recalc
	_colorEmitters.insert(it, newEmitter);

	debugPrintf(DEBUG_INSANE, "Added emitter %s at la %u", emitter.getName(), outputLocalAddress);

	// And then recalculate all slopes
	for (std::vector<std::shared_ptr<componentEmitter> >::iterator itThisEmitter = _colorEmitters.begin();
	     itThisEmitter < _colorEmitters.end();
	     ++itThisEmitter) {
		std::shared_ptr<componentEmitter> spNextEmitter;

		// Slope is to the next emitter, except the last one wraps around to the
		// first
		if (*itThisEmitter != _colorEmitters.back()) {
			spNextEmitter = *(std::next(itThisEmitter));
		} else {
			spNextEmitter = *(_colorEmitters.begin());
		}

		// I hate this mix of iterators to shared pointers and shared pointers,
		// but it works
		(*itThisEmitter)->slope = (spNextEmitter->emitter->getV() - (*itThisEmitter)->emitter->getV())
		                          / (spNextEmitter->emitter->getU() - (*itThisEmitter)->emitter->getU());
	}
}

float CompositeModule::getAngle(int num) {
	return _colorEmitters[num]->angle;
}

std::vector<outputEmitter>CompositeModule::Hue2EmitterPower(const HSIColor& HSI) const {
	float H = fmod(HSI.getHue() + 360, 360);
	float S = HSI.getSaturation();
	float I = HSI.getIntensity();

	float tanH = tan(M_PI * fmod(H, 360) / (float)180); // Get the tangent since we will use it often.

	std::shared_ptr<componentEmitter> emitter1;
	std::shared_ptr<componentEmitter> emitter2;

	// Check the range to determine which intersection to do.
	// For angle less than the smallest CIE hue or larger than the largest,
	// special case.

	std::shared_ptr<componentEmitter>::iterator it;
	it = std::find_if(_colorEmitters.begin(), _colorEmitters.end(),
	                  [H](const std::shared_ptr<componentEmitter> e) -> bool {
		return e->angle < H;
	});

	// If it is the last emitter, wrap around to use last:first
	// If no match was found, H is less than the first; also use last:first
	if ((it == _colorEmitters.end()) || ((it + 1) == _colorEmitters.end())) {
		emitter1 = _colorEmitters.back();
		emitter2 = _colorEmitters.front();
	} else {
		emitter1 = *it++;
		emitter2 = *it;
	}

	debugPrintf(DEBUG_INSANE, "Hue %f: Emitter 1: %s, Emitter 2: %s", H,
	            emitter1.emitter->getName(), emitter2.emitter->getName());

	// Get the ustar and vstar values for the target LEDs.
	float emitter1_ustar = emitter1.emitter->emitter->getU() - _whiteEmitter.emitter->getU();
	float emitter1_vstar = emitter1.emitter->emitter->getV() - _whiteEmitter.emitter->getV();
	float emitter2_ustar = emitter2.emitter->emitter->getU() - _whiteEmitter.emitter->getU();
	float emitter2_vstar = emitter2.emitter->emitter->getV() - _whiteEmitter.emitter->getV();

	debugPrintf(DEBUG_INSANE,
	            "t: %f, e1u: %f, e1v: %f, e2u: %f, e2v: %f",
	            tanH,
	            emitter1_ustar,
	            emitter1_vstar,
	            emitter2_ustar,
	            emitter2_vstar);

	// Get the slope between LED1 and LED2.
	float slope = emitter1.emitter->slope;

	float ustar = (emitter2_vstar - slope * emitter2_ustar) / (tanH - slope);
	float vstar = tanH / (slope - tanH) * (slope * emitter2_ustar - emitter2_vstar);

	debugPrintf(DEBUG_INSANE, "s: %f, u: %f, v: %f", slope, ustar, vstar);

	// Calculate separately because abs() is a goddamned macro that returns
	// goddamned integers which goddamned breaks
	float emitter1power = I * S * fabs(ustar - emitter2_ustar) / fabs(emitter2_ustar - emitter1_ustar);
	float emitter2power = I * S * fabs(ustar - emitter1_ustar) / fabs(emitter2_ustar - emitter1_ustar);

	// Build our output emitters, all of which are off (0.0f) except the two we
	// just found and computed

	debugPrintf(DEBUG_INSANE, "I: %3.2f S: %3.2f u: %f v: %f n1: %f d: %f p1: %f p2: %f", I, S,
	            ustar, vstar, nom1, denom, emitter1Power, emitter2Power);

	// Copy our color emitters with default power of zero
	std::vector<outputEmitter> emitterPowers;
	float emitterPower = 0.0f;

	for (std::vector<std::shared_ptr<componentEmitter> >::const_iterator itspEmitter = _colorEmitters.begin();
	     itspEmitter < _colorEmitters.end();
	     ++itspEmitter) {

		if ((*itspEmitter) == emitter1) {
			emitterPower = emitter1Power;
		} else if ((*itspEmitter) == emitter2) {
			emitterPower = emitter2Power;
		} else {
			emitterPower = 0.0f;
		}
		outputEmitter o((*itspEmitter)->outputLocalAddress, emitterPower);

		emitterPowers.push_back(o);
		debugPrintf(DEBUG_INSANE, "CompositeModule::Hue2EmitterPower added emitterPower[%u] at la %u with power %f",
		            emitterPowers.size() - 1, emitterPowers.back().outputLocalAddress,
		            emitterPowers.back().power);
	}

	// Add white to the end, and set the power
	emitterPowers.push_back(outputEmitter(_whiteEmitter.outputLocalAddress, I * (1 - S)));

	return emitterPowers;
}
