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
#include "modes.h"
#include "luminous-particle.h"

#include "mode.h"
#include "hsilight.h"
#include "hsicolor.h"
#include "ldebug.h"

#include "Particle.h"

#include <math.h>
#include <algorithm>

const int modeCount = 4;

Mode *modes[modeCount] = {
  new ModeOff("Off"),
  new ModeTest("Test"),
  new ModeRotate("Rotate"),
  new ModeE131("E131")
};

uint16_t currentMode = 0;

bool ModeOff::start(std::vector<std::shared_ptr<HSILight> >lights) {
  debugPrintf(DEBUG_INSANE, "startOff: turning all lights off");
  std::for_each(lights.begin(),
                lights.end(),
                [&](std::shared_ptr<HSILight>light) {
    light->setColor(HSIColor(0, 0, 0));
  });

  return true;
}

bool ModeTest::run(std::vector<std::shared_ptr<HSILight> >lights,
                   bool                                   lightsMustUpdate) {
  const unsigned int   millisPerColor = 2000;
  static unsigned long firstChange    = millis();
  static unsigned int  counter        = 0;
  static unsigned int  lastCounter    = 0;

  // static char msg[200];

  counter = floor((millis() - firstChange) / millisPerColor);

  // sprintf(msg,"millis %d, firstchange %d, millisperColor %u, counter
  // %d",millis(),firstChange,millisPerColor,counter);
  // debugPrint(DEBUG_TRACE, msg);
  if (lightsMustUpdate || (counter != lastCounter)) {
    std::for_each(lights.begin(),
                  lights.end(),
                  [&](std::shared_ptr<HSILight>val) {
      debugPrintf(DEBUG_INSANE, "effectTest: %u on", counter);
      val->setSingleEmitterOn(counter);
    });
    lastCounter = counter;
  }
  return true;
}

bool ModeRotate::start(std::vector<std::shared_ptr<HSILight> >lights) {
  _rotateColor = HSIColor(0, 1.0f, 0.5f);
  debugPrintf(DEBUG_INSANE, "Rotate: setting colors");
  std::for_each(lights.begin(),
                lights.end(),
                [&](std::shared_ptr<HSILight>light) {
    light->setColor(_rotateColor);
  });
  return true;
}

bool ModeRotate::doRotate() {
  const unsigned int millisPerHueRotation = 1000 * 60;
  const unsigned int millisPerSatRotation = millisPerHueRotation * 2;

  const unsigned int minMillis = 100;

  static double hue = 0.0f;
  static double sat = 1.0f;

  static signed int satDir        = -1;
  static unsigned long lastMillis = millis();

  unsigned long nowMillis = millis();

  unsigned long millisPassed = nowMillis - lastMillis;

  if (millisPassed < minMillis) return true;

  lastMillis = nowMillis;

  double hueToAdd = 360.0f *
                    ((double)millisPassed / (double)millisPerHueRotation);
  hue  = fmod(hue + hueToAdd + 360.0f, 360);
  sat += ((double)millisPassed / (double)millisPerSatRotation) * satDir;

  if (sat > 1.0f) {
    sat    = 1.0f;
    satDir = -1;
  } else if (sat < 0.0f) {
    sat    = 0.0f;
    satDir = 1;
  }
  debugPrintf(DEBUG_INSANE,
              "H: %3.0f (+ %3.3f, %lu) S: %3.2f I: 0.5f",
              hue,
              hueToAdd,
              millisPassed,
              sat);
  _rotateColor.setHSI(hue, sat, 0.5f);
  return true;
}

bool ModeRotate::run(std::vector<std::shared_ptr<HSILight> >lights,
                     bool                                   lightsMustUpdate) {
  doRotate();

  std::for_each(lights.begin(), lights.end(), [&](std::shared_ptr<HSILight>val) {
    val->setColor(_rotateColor);
  });

  return true;
}

bool ModeE131::start(std::vector<std::shared_ptr<HSILight> >lights) {
  // TODO: ensure data and lights are initialized to off
  // TODO: send signal?
  return true;
}

bool ModeE131::run(std::vector<std::shared_ptr<HSILight> >lights,
                   bool                                   lightsMustUpdate) {
  debugPrint(DEBUG_INSANE, "effectModeE131: starting");

  std::for_each(lights.begin(), lights.end(), [&](std::shared_ptr<HSILight>light) {
    // debugPrintf(DEBUG_TRACE, "effectModeE131: %s on", light->getName());

    const std::shared_ptr<E131>e131 = light->getE131();
    const uint16_t e131LocalAddress = light->getE131LocalAddress();

    uint8_t moderaw = e131->data[e131LocalAddress];
    float hueraw = twoBytesToFloat(e131->data, e131LocalAddress + 1);
    float sat = twoBytesToFloat(e131->data, e131LocalAddress + 3);
    float lum = twoBytesToFloat(e131->data, e131LocalAddress + 5);

    HSIColor color(360.0f * hueraw, sat, lum);

    debugPrintf(DEBUG_INSANE, "e131 data for %s: mode %u (%4.4f, %4.4f, %4.4f)",
                light->getName(), moderaw, color.getHue(), color.getSaturation(),
                color.getIntensity());

    light->setColor(color);
  });
  return true;
}

bool ModeE131::end(std::vector<std::shared_ptr<HSILight> >lights) {
  // TODO: send signal?
  return true;
}

bool setModeByNumber(uint16_t modeNumber) {
  if (modeNumber >= modeCount) return false;

  modes[currentMode]->end(allLights);
  currentMode = modeNumber;
  modes[currentMode]->start(allLights);
  lightsMustUpdate  = true;
  displayMustUpdate = true;
  return true;
}

bool setModeByName(String modeName) {
  debugPrintf(DEBUG_TRACE, "setModeByName: start for %s", modeName.c_str());

  for (uint16_t i = 0; i < modeCount; i++) {
    if (modes[i]->getName() == modeName) {
      modes[currentMode]->end(allLights);
      currentMode = i;
      modes[currentMode]->start(allLights);
      lightsMustUpdate  = true;
      displayMustUpdate = true;
      return true;
    }
  }
  debugPrintf(DEBUG_ERROR, "setModeByName: no such mode %s", modeName.c_str());
  return false;
}

void nextMode() {
  modes[currentMode]->end(allLights);
  currentMode = ((currentMode + 1) % modeCount);
  modes[currentMode]->start(allLights);
  lightsMustUpdate  = true;
  displayMustUpdate = true;
}

void runCurrentMode() {
  modes[currentMode]->run(allLights, lightsMustUpdate);
  lightsMustUpdate = false;
}

const String getCurrentModeName() {
  return modes[currentMode]->getName();
}

float twoBytesToFloat(const uint8_t *buf, uint16_t offset) {
  return (float)(buf[offset] << 8 | buf[offset + 1]) / 65535.0f;
}
