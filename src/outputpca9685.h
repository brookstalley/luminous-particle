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
#ifndef OUTPUTPCA9685_H
#define OUTPUTPCA9685_H

#include "outputinterface.h"
#include "PCA9685-Particle/PCA9685.h"
#include "emitter.h"

#include "Particle.h"

#include <vector>

class OutputPCA9685: public OutputInterface {
private:

	TwoWire& _i2cbus;
	PCA9685 _pwm;
	uint8_t _i2caddress;

	bool initImplementation();
	bool sleepImplementation() const;
	bool allOffImplementation() const;

public:

	OutputPCA9685(TwoWire & i2cbus, uint8_t i2caddress);

	bool setEmitterPowers(const std::vector < outputEmitter >& emitters,
	                      float scaleFactor);

	bool init();
	bool sleep() const;
	bool allOff() const;
};

#endif /* ifndef OUTPUTPCA9685_H */
