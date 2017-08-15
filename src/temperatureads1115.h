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
// along with Luminous.  If not, see <http://www.gnu.org/licenses/>.
//
//**********************************************************
#ifndef TEMPERATUREADS1115_H
#define TEMPERATUREADS1115_H

#include "temperatureinterface.h"
#include "Adafruit_ADS1X15/Adafruit_ADS1015.h"

#include "Particle.h"

class TemperatureAds1115: public TemperatureInterface {
private:

	TwoWire& _i2cbus;
	Adafruit_ADS1015 _adc;
	uint8_t _i2caddress;

	// Note that this class assumes all thermistors on the same ADC have the same
	// characteristics
	uint16_t _seriesResistor;
	uint16_t _adcMax;
	uint16_t _thermistorNominal;
	uint16_t _temperatureNominal;
	uint16_t _bCoef;

	bool beginImplementation();

public:

	TemperatureAds1115(TwoWire & i2cbus,
	                   uint8_t i2caddress,
	                   uint16_t seriesResistor,
	                   uint16_t thermistorNominal,
	                   uint16_t temperatureNominal,
	                   uint16_t bCoef);
	float getTemperature(uint16_t temperatureLocalAddress);
	bool  begin();
};

#endif /* ifndef TEMPERATUREADS1115_H */
