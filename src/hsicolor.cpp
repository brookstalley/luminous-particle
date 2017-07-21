//*********************************************************
//
// TeensyLED Controller Library
// Copyright Brian Neltner 2015
// Version 0.1 - April 13, 2015
//
// This file is part of TeensyLED Controller.
//
// TeensyLED Controller is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// TeensyLED Controller is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//
//**********************************************************

#include <math.h>
#include "hsicolor.h"

HSIColor::HSIColor(float hue, float saturation, float intensity) {
  setHue(hue);
  setSaturation(saturation);
  setIntensity(intensity);
}

// Default constructor.
HSIColor::HSIColor(void) {
  setHue(0);
  setSaturation(0);
  setIntensity(0);
}

void HSIColor::setHue(float hue) {
  _hue = fmod(hue, 360);
}

void HSIColor::setSaturation(float saturation) {
  _saturation = saturation>0?(saturation<1?saturation:1):0;
}

void HSIColor::setIntensity(float intensity) {
  _intensity = intensity>0?(intensity<1?intensity:1):0;
}

void HSIColor::setHSI(float hue, float saturation, float intensity) {
  setHue(hue);
  setSaturation(saturation);
  setIntensity(intensity);
}

float HSIColor::getHue(void) {
  return _hue;
}

float HSIColor::getSaturation(void) {
  return _saturation;
}

float HSIColor::getIntensity(void) {
  return _intensity;
}

void HSIColor::getHSI(float *HSI) {
  HSI[0] = getHue();
  HSI[1] = getSaturation();
  HSI[2] = getIntensity();
}
