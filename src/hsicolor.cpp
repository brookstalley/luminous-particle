// *********************************************************

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
// along with Luminous.  If not, see <http://www.gnu.org/licenses/>.
//
// **********************************************************

#include <math.h>
#include "hsicolor.h"

#include "ldebug.h"

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
  _saturation = saturation > 0 ? (saturation < 1 ? saturation : 1) : 0;
}

void HSIColor::setIntensity(float intensity) {
  _intensity = intensity > 0 ? (intensity < 1 ? intensity : 1) : 0;
}

void HSIColor::setHSI(float hue, float saturation, float intensity) {
  setHue(hue);
  setSaturation(saturation);
  setIntensity(intensity);
}

void HSIColor::setRGB(float red, float green, float blue) {
  _intensity = (red + green + blue) / 3.0f;

  double rn = red / (red + green + blue);
  double gn = green / (red + green + blue);
  double bn = blue / (red + green + blue);

  _hue =
    acos((0.5 * ((rn - gn) + (rn - bn))) /
         (sqrt((rn - gn) * (rn - gn) + (rn - bn) * (gn - bn))));

  if (blue > green) {
    _hue = 360.0f - _hue;
  }
  _saturation = 1.0f - 3 * min(rn, min(gn, bn));
  debugPrintf(DEBUG_INSANE,
              "HSIColor::setRGB converted (%4.4f, %4.4f, %4.4f) to (%4.4f, %4.4f, %4.4f)",
              red,
              green,
              blue,
              _hue,
              _saturation,
              _intensity);
}

void HSIColor::setHLS(float hslHue, float hslLightness, float hslSaturation) {
  // Formula from http://codeitdown.com/hsl-hsb-hsv-color/
  _hue       = hslHue;
  _intensity = ((2.0f * hslLightness) + hslSaturation * (1.0f - fabs(2.0f * hslLightness - 1.0f))) / 2.0f;

  if (_intensity == 0.0f) {
    _saturation = 0.0f;
  } else {
    _saturation = 2.0f * (_intensity - hslLightness) / _intensity;
  }

  debugPrintf(DEBUG_INSANE,
              "HSIColor::setHLS converted (%4.4f, %4.4f, %4.4f) to (%4.4f, %4.4f, %4.4f)",
              hslHue,
              hslLightness,
              hslSaturation,
              _hue,
              _saturation,
              _intensity);
}

const float HSIColor::getHue(void) const {
  return _hue;
}

const float HSIColor::getSaturation(void) const {
  return _saturation;
}

const float HSIColor::getIntensity(void) const {
  return _intensity;
}

void HSIColor::getHSI(float *HSI) {
  HSI[0] = getHue();
  HSI[1] = getSaturation();
  HSI[2] = getIntensity();
}

bool HSIColor::operator==(const HSIColor& other) const {
  if ((_hue == other._hue) && (_saturation == other._saturation) &&
      (_intensity == other._intensity)) return true;

  return false;
}
