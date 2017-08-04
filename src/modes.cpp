#include "modes.h"
#include "luminous-particle.h"

#include "mode.h"
#include "hsilight.h"
#include "hsicolor.h"
#include "debug.h"

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
  std::for_each(lights.begin(), lights.end(), [&](std::shared_ptr<HSILight>light) {
    light->setColor(HSIColor(0, 0, 0));
  });

  return true;
}

bool ModeTest::run(std::vector<std::shared_ptr<HSILight> >lights,
                   bool lightsMustUpdate) {
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
    std::for_each(lights.begin(), lights.end(), [&](std::shared_ptr<HSILight>val) {
      debugPrintf(DEBUG_INSANE, "effectTest: %u on", counter);
      val->setSingleEmitterOn(counter);
    });
    lastCounter = counter;
  }
  return true;
}

bool ModeRotate::run(std::vector<std::shared_ptr<HSILight> >lights, bool lightsMustUpdate) {
  return true;

  const unsigned int millisPerHueRotation = 1000 * 60;
  const unsigned int millisPerSatRotation = millisPerHueRotation * 2;

  const unsigned int minMillis = 100;

  static double hue = 0.0f;
  static double sat = 1.0f;

  static signed int    satDir     = -1;
  static unsigned long lastMillis = millis();
  static HSIColor      rotateColor;

  unsigned long nowMillis = millis();

  unsigned long millisPassed = nowMillis - lastMillis;

  if (millisPassed < minMillis) return true;

  lastMillis = nowMillis;

  double hueToAdd = 360.0f * ((double)millisPassed / (double)millisPerHueRotation);
  hue  = fmod(hue + hueToAdd + 360.0f, 360);
  sat += ((double)millisPassed / (double)millisPerSatRotation) * satDir;

  if (sat > 1.0f) {
    sat    = 1.0f;
    satDir = -1;
  } else if (sat < 0.0f) {
    sat    = 0.0f;
    satDir = 1;
  }
  debugPrintf(DEBUG_INSANE, "H: %3.0f (+ %3.3f, %lu) S: %3.2f I: 1.0", hue, hueToAdd, millisPassed, sat);

  rotateColor.setHSI(hue, sat, 0.5f);
  std::for_each(lights.begin(), lights.end(), [&](std::shared_ptr<HSILight>val) {
    val->setColor(rotateColor);
  });

  return true;
}

bool ModeE131::start(std::vector<std::shared_ptr<HSILight> >lights) {
  // TODO: ensure data and lights are initialized to off
  // TODO: send signal?
  return true;
}

bool ModeE131::run(std::vector<std::shared_ptr<HSILight> >lights,
                   bool lightsMustUpdate) {
  // TODO: checksum data to see if it changed

  debugPrint(DEBUG_INSANE, "effectModeE131: starting");

  std::for_each(lights.begin(), lights.end(), [&](std::shared_ptr<HSILight>val) {
    // debugPrintf(DEBUG_INSANE, "effectModeE131: %u on", counter);
    val->setColorFromE131();
  });
  return true;
}

bool ModeE131::end(std::vector<std::shared_ptr<HSILight> >lights) {
  // TODO: send signal?
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

const char* getCurrentModeName() {
  return modes[currentMode]->getName();
}
