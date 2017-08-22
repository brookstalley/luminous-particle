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

#include "pages.h"
#include "display.h"
#include "modes.h"
#include "debug.h"
#include "hsicolor.h"
#include "particlefunctions.h"
#include "luminous-particle.h"

char* TimeToString(unsigned long t)
{
  static char str[12];
  long h = t / 3600;

  t = t % 3600;
  int m = t / 60;
  int s = t % 60;
  sprintf(str, "%02ld:%02d:%02d", h, m, s);
  return str;
}

bool StatusPage::render() {
  Page::render();
  update();
  return true;
}

bool StatusPage::update() {
  display.setTop();
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "Running:    %s",
                  TimeToString(millis() / 1000));

  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "Mode:       %s", getCurrentModeName());

  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "Loops/s:    %u",
                  loopsPerSecond);

  if (lastBrightnessRemote) {
    display.println(DISPLAY_WHITE, DISPLAY_BLACK, "Brightness: [%2.0f%%]",
                    globalBrightness * 100);
  } else {
    display.println(DISPLAY_WHITE, DISPLAY_BLACK,
                    "Brightness: %2.0f%%",
                    globalBrightness * 100);
  }

  char debugName[12];
  getDebugLevelName(getDebugLevel(), debugName, sizeof(debugName));
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "Debug:      %s", debugName);

  if (wifiCurrentState == WIFI_CONNECTED) {
    display.println(DISPLAY_WHITE, DISPLAY_BLACK, "WiFi:       %s", WiFi.SSID());

    // It is insane that the following line does not compile.
    // debugPrintf(DEBUG_TRACE, "  server=%s:%u", WiFi.localIP(),
    // mainUniverse->getUdpPort());
    IPAddress localAddr = WiFi.localIP();
    byte oct1           = localAddr[0];
    byte oct2           = localAddr[1];
    byte oct3           = localAddr[2];
    byte oct4           = localAddr[3];
    display.println(DISPLAY_WHITE, DISPLAY_BLACK, "     %d.%d.%d.%d", oct1, oct2, oct3, oct4);
  } else {
    display.println(DISPLAY_WHITE, DISPLAY_BLACK, "WiFi:       Offline");
  }

  if (particleCurrentState == PARTICLE_CONNECTED) {
    display.println(DISPLAY_WHITE, DISPLAY_BLACK, "Particle:   Online");
  } else {
    display.println(DISPLAY_WHITE, DISPLAY_BLACK, "Particle:   Offline");
  }

  display.println(DISPLAY_WHITE, DISPLAY_BLACK,
                  (particleDesiredState != particleCurrentState ?
                   (particleDesiredState ==
                    PARTICLE_CONNECTED ?
                    "            Connecting" : "            Disconnecting")
                   : ""));

  return true;
}

LightPage::LightPage() {
  // When constructed, we point to the first light
  _itsplight = allLights.begin();
}

bool LightPage::render() {
  // Assume that we either were already on this page, or someone else called display.clear
  Page::render();
  return true;
}

bool LightPage::update() {
  display.setTop();

  HSIColor *color;
  (*_itsplight)->getColor(color);
  display.println(DISPLAY_CYAN,  DISPLAY_BLACK, "Light %s",        (*_itspLight)->getName());
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "  E131 Addr: %u", (*_itsplight)->getE131LocalAddress());
  display.println(DISPLAY_WHITE,
                  DISPLAY_BLACK,
                  "  HSI: %4.4f, %4.4f, %4.4f",
                  color->getHue(),
                  color->getSaturation,
                  color->getIntensity);
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "%s",         (*_itsplight)->getDiagnostic());
  display.println(DISPLAY_WHITE, DISPLAY_BLCAK, "  Temp: %f", (*_itsplight)->getTempterature());
}

void LightPage::nextButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "LightPage::nextButton (%i)", clicks);

  if (++_itspLight == allLights.end()) _itspLight = allLights.begin();
  render();
}

void LightPage::prevButton(int clicks) {
  if (_itspLight == allLights.begin()) {
    _itspLight = allLights.end();
  }
  _itsplight--;
  render();
}

void LightPage::selectButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "LightPage::selectButton (%i)", clicks);
  currentPage = *_selectedItem;
  currentPage->render();
}
