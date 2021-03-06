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
#ifndef LUMINOUS_PARTICLE_H
#define LUMINOUS_PARTICLE_H

#include "display.h"
#include "HSILight.h"
#include "page.h"
#include "ResponsiveAnalogRead/ResponsiveAnalogRead.h"

#include <vector>
#include <memory>

#define DEBUG_BUILD

#define spi_pin_sclk A3
#define spi_pin_mosi A5
#define spi_pin_dc   D7
#define spi_pin_cs   A2
#define spi_pin_rst  D5

#define i2c_pin_scl D1
#define i2c_pin_sda D0

#define TEMP_PIN A1
#define BRIGHTNESS_PIN  A0
#define BACK_BUTTON_PIN D4
#define PREV_BUTTON_PIN D3
#define NEXT_BUTTON_PIN D2
#define SELECT_BUTTON_PIN A1

extern float globalBrightness;
extern bool  lastBrightnessRemote;

extern float LEDTempCelsius;

extern bool displayMustUpdate;
extern bool lightsMustUpdate;
extern unsigned int loopsPerSecond;


extern ResponsiveAnalogRead brightnessControl;
extern std::vector<std::shared_ptr<HSILight> > allLights;
extern std::vector<std::shared_ptr<Page> > pageStack;
extern Display display;

bool setupDisplay();
void setup(void);
void loop();
void modeOff();
void modeTest();
void modeRotate();
void modeE131();

#endif /* ifndef LUMINOUS_PARTICLE_H */
