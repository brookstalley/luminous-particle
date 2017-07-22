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

#ifndef HSICOLOR_H
#define HSICOLOR_H

class HSIColor {
  private:
    float _hue, _saturation, _intensity;
  public:
    HSIColor(float hue, float saturation, float intensity);
    HSIColor(void);
    void setHue(float hue);
    void setSaturation(float saturation);
    void setIntensity(float intensity);
    void setHSI(float hue, float saturation, float intensity);
    float getHue(void);
    float getSaturation(void);
    float getIntensity(void);
    void getHSI(float *HSI);
};

#endif
