// *********************************************************

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
// **********************************************************
#include "temperatureads1115.h"
#include "temperatureinterface.h"
#include "ldebug.h"
#include "Particle.h"

#include "Adafruit_ADS1X15/Adafruit_ADS1015.h"

#include <math.h>

#define ABS_ZERO 273.15

TemperatureAds1115::TemperatureAds1115(TwoWire& i2cbus,
                                       uint8_t  i2caddress,
                                       uint16_t seriesResistor,
                                       uint16_t thermistorNominal,
                                       uint16_t temperatureNominal,
                                       uint16_t bCoef) :
  _i2cbus(i2cbus),
  _i2caddress(i2caddress),
  _seriesResistor(seriesResistor),
  _adcMax(65535),
  _thermistorNominal(thermistorNominal),
  _temperatureNominal(temperatureNominal),
  _bCoef(bCoef) {}

bool TemperatureAds1115::beginImplementation() {
  {
    debugPrint(DEBUG_TRACE, "TemperatureAds1115::beginImplementation");

    _adc.setSPS(ADS1115_DR_8SPS);
    _adc.startContinuous_SingleEnded(0);
    _adc.startContinuous_SingleEnded(1);
    _adc.startContinuous_SingleEnded(2);
    _adc.startContinuous_SingleEnded(3);

    // make sure it has enough time before someone starts asking it for readings
    delay(10);

    // ads.startContinuous_SingleEnded(1);
    // ads.startContinuous_SingleEnded(2);
    // ads.startContinuous_SingleEnded(3);
    // ads.startContinuous_Differential_0_1();
    // ads.startContinuous_Differential_0_3();
    // ads.startContinuous_Differential_1_3();
    // ads.startContinuous_Differential_2_3();

    return true;
  }
}

float TemperatureAds1115::getTemperature(uint16_t temperatureLocalAddress) {
  float rawReading = _adc.readADC_SingleEnded(temperatureLocalAddress);

  float voltage    = rawReading * (5.0 / _adcMax);
  float refCurrent = 0.0001f;
  float resistance = (voltage / refCurrent);
  float ln         = log(resistance / _thermistorNominal);
  float kelvin     = 1 /
                     (0.0033540170 + (0.00025617244 * ln) + (0.0000021400943 * ln * ln) +
                      (-0.000000072405219 * ln * ln * ln));
  float temp = kelvin - 273.15;

  float r = ((float)_seriesResistor) / (((float)_adcMax) / rawReading - 1);
  float s = (r / ((float)_thermistorNominal));

  s = log(s);
  s = s * (1 / ((float)_bCoef));
  s = s + 1 / (((float)_temperatureNominal) + ABS_ZERO);
  s = 1 / s;

  debugPrintf(DEBUG_TRACE, "TemperatureAds1115::getTemperature raw=%5.1f r=%5.1f s=%5.1f", rawReading, r, s);
  debugPrintf(DEBUG_TRACE,
              "TemperatureAds1115::getTemperature voltage=%5.1f resist=%5.1f ln=%5.1f kelvin=%5.1f t=%f.1f",
              voltage,
              resistance,
              ln,
              kelvin,
              temp);

  return s;
}

bool TemperatureAds1115::begin() {
  return beginImplementation();
}
