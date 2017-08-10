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

	// Create our component emitter based on the prototype emitter, plus this
	// one's particular address
	std::shared_ptr<componentEmitter> newEmitter =
		std::make_shared<componentEmitter>(componentEmitter(&emitter, outputLocalAddress, newAngle, 0));

	// Note that we add the emitter at the end without regard for order
	// CompositeModule WILL NOT WORK unless calculate() is called after adding last emitter
	_colorEmitters.push_back(newEmitter);

	debugPrintf(DEBUG_INSANE, "Added emitter %s at la %u", emitter.getName(), outputLocalAddress);
}

void CompositeModule::calculate() {
	// Sort the emitters by angle
	std::sort(_colorEmitters.begin(), _colorEmitters.end(),
	          [](std::shared_ptr < componentEmitter > a, std::shared_ptr < componentEmitter > b) {
		return b->angle < a->angle;
	}
	          );

	// Recalculate all slopes
	for (auto it = _colorEmitters.begin(); it != _colorEmitters.end(); it++) {
		std::shared_ptr<componentEmitter> spNextEmitter;

		// Slope is to the next emitter, except the last one wraps around to the
		// first
		if ((*it) != _colorEmitters.back()) {
			spNextEmitter = *(std::next(it));
		} else {
			spNextEmitter = _colorEmitters.front();
		}

		// I hate this mix of iterators to shared pointers and shared pointers,
		// but it works
		(*it)->slope = (spNextEmitter->emitter->getV() - (*it)->emitter->getV())
		               / (spNextEmitter->emitter->getU() - (*it)->emitter->getU());
	}
}

float CompositeModule::getAngle(int num) {
	return _colorEmitters[num]->angle;
}

std::vector<outputEmitter>CompositeModule::emitterPowersFromHSI(const HSIColor& HSI) const {
	float H = fmod(HSI.getHue() + 360, 360);
	float S = HSI.getSaturation();
	float I = HSI.getIntensity();

	float tanH = tan(M_PI * fmod(H, 360) / (float)180); // Get the tangent since we will use it often.

	std::shared_ptr<componentEmitter> emitter1;
	std::shared_ptr<componentEmitter> emitter2;

	// Check the range to determine which intersection to do.
	// For angle less than the smallest CIE hue or larger than the largest,
	// special case.

	std::vector<std::shared_ptr<componentEmitter> >::const_iterator it = std::find_if(_colorEmitters.begin(), _colorEmitters.end(),
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
	            emitter1->emitter->getName(), emitter2->emitter->getName());

	// Get the ustar and vstar values for the target LEDs.
	float emitter1_ustar = emitter1->emitter->getU() - _whiteEmitter.emitter->getU();
	//unused: float emitter1_vstar = emitter1->emitter->getV() - _whiteEmitter.emitter->getV();
	float emitter2_ustar = emitter2->emitter->getU() - _whiteEmitter.emitter->getU();
	float emitter2_vstar = emitter2->emitter->getV() - _whiteEmitter.emitter->getV();

	// Get the slope between LED1 and LED2.
	float slope = emitter1->slope;

	float ustar = (emitter2_vstar - slope * emitter2_ustar) / (tanH - slope);
	float vstar = tanH / (slope - tanH) * (slope * emitter2_ustar - emitter2_vstar);

	debugPrintf(DEBUG_INSANE, "s: %f, u: %f, v: %f", slope, ustar, vstar);

	// Calculate separately because abs() is a goddamned macro that returns
	// goddamned integers which goddamned breaks
	float emitter1power = I * S * fabs(ustar - emitter2_ustar) / fabs(emitter2_ustar - emitter1_ustar);
	float emitter2power = I * S * fabs(ustar - emitter1_ustar) / fabs(emitter2_ustar - emitter1_ustar);

	// Build our output emitters, all of which are off (0.0f) except the two we
	// just found and computed
/*
        debugPrintf(DEBUG_INSANE, "t: %f, e1u: %f, e1v: %f, e2u: %f, e2v: %f I: %3.2f S: %3.2f u: %f v: %f n1: %f d: %f p1: %f p2: %f"
                ,	tanH,
        emitter1_ustar,
        emitter1_vstar,
        emitter2_ustar,
        emitter2_vstar, I, S,
                    ustar, vstar, nom1, denom, emitter1Power, emitter2Power);*/

	// Copy our color emitters with default power of zero
	std::vector<outputEmitter> emitterPowers;
	float emitterPower = 0.0f;

	for (std::vector<std::shared_ptr<componentEmitter> >::const_iterator itspEmitter = _colorEmitters.begin();
	     itspEmitter < _colorEmitters.end();
	     ++itspEmitter) {

		if ((*itspEmitter) == emitter1) {
			emitterPower = emitter1power;
		} else if ((*itspEmitter) == emitter2) {
			emitterPower = emitter2power;
		} else {
			emitterPower = 0.0f;
		}
		outputEmitter o((*itspEmitter)->outputLocalAddress, emitterPower);

		emitterPowers.push_back(o);
		debugPrintf(DEBUG_INSANE, "CompositeModule::emitterPowersFromHSI added emitterPower[%u] at la %u with power %f",
		            emitterPowers.size() - 1, emitterPowers.back().outputLocalAddress,
		            emitterPowers.back().power);
	}

	// Add white to the end, and set the power
	emitterPowers.push_back(outputEmitter(_whiteEmitter.outputLocalAddress, I * (1 - S)));

	return emitterPowers;
}
