#include "effects.h"

bool effectTest(std::vector<std::shared_ptr<HSILight>> lights, bool lightsMustUpdate) {
  const unsigned int millisPerColor = 2000;
  static unsigned long firstChange = millis();
  static unsigned int counter = 0;
  static unsigned int lastCounter = 0;
  //static char msg[200];

  counter = floor((millis() - firstChange) / millisPerColor);

  //sprintf(msg,"millis %d, firstchange %d, millisperColor %u, counter %d",millis(),firstChange,millisPerColor,counter);
  //debugPrint(msg);
  if (lightsMustUpdate || (counter != lastCounter)) {
    std::for_each(lights.begin(), lights.end(), [&] (std::shared_ptr<HSILight> val) {
      val->debugOutput();
      char msg[30];
      sprintf(msg,"effectTest: %u on",counter);
      val->setSingleEmitterOn(counter);
    });
    lastCounter = counter;
  }
  return true;
}

bool effectRotate(std::vector<std::shared_ptr<HSILight>> lights, bool lightsMustUpdate) {
  return true;
  
  const unsigned int millisPerHueRotation = 1000 * 60;
  const unsigned int millisPerSatRotation = millisPerHueRotation *2;

  const unsigned int minMillis=100;

  static double hue = 0.0f;
  static double sat = 1.0f;

  static signed int satDir = -1;
  static unsigned long lastMillis = millis();
  static HSIColor rotateColor;

  unsigned long nowMillis = millis();

  unsigned long millisPassed = nowMillis - lastMillis;
  if (millisPassed < minMillis)
    return true;

  lastMillis = nowMillis;

  double hueToAdd = 360.0f * ((double)millisPassed / (double)millisPerHueRotation);
  hue = fmod(hue + hueToAdd + 360.0f,360);
  sat += ((double)millisPassed / (double)millisPerSatRotation) * satDir;
  if (sat > 1.0f) {
    sat = 1.0f;
    satDir = -1;
  } else if (sat < 0.0f) {
    sat = 0.0f;
    satDir = 1;
  }
  char msg[30];
  sprintf(msg,"H: %3.0f (+ %3.3f, %lu) S: %3.2f I: 1.0", hue, hueToAdd, millisPassed, sat);
  debugPrint(msg);
  rotateColor.setHSI(hue, sat, 0.5f);
  std::for_each(lights.begin(), lights.end(), [&] (std::shared_ptr<HSILight> val) {
    val->setColor(rotateColor);
  });

  return true;
}
