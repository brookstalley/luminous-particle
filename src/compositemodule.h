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

#ifndef COMPOSITEMODULE_H
#define COMPOSITEMODULE_H

#include "emitter.h"
#include "hsicolor.h"

#include "Particle.h"

#include <vector>

struct componentEmitter {
	componentEmitter(const Emitter * e, uint16_t la, float a, float s) {
		emitter            = e;
		outputLocalAddress = la;
		angle              = a;
		slope              = s;
	}

	componentEmitter() {
	}

	const Emitter *emitter;
	uint16_t outputLocalAddress;
	float angle;
	float slope;
};

class CompositeModule {
private:

	std::vector < std::shared_ptr < componentEmitter >> _colorEmitters;
	componentEmitter _whiteEmitter;
	float _dimTemperature;
	float _shutdownTemperature;

public:

	CompositeModule(float dimTemperature, float shutdownTemperature);
	void  addWhiteEmitter(const Emitter& white,
	                      uint16_t outputLocalAddress);
	void  addColorEmitter(const Emitter& emitter,
	                      uint16_t outputLocalAddress);
	void prepare();
	float getAngle(int emitternum);
	float getSlope(int emitternum);
	std::vector < outputEmitter > Hue2EmitterPower(const HSIColor &HSI) const;
};

#endif /* ifndef COMPOSITEMODULE_H */
