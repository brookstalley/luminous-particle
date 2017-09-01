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
#include "menus.h"
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

char* IPAddressToString(IPAddress i) {
  static char str[15];

  byte oct1 = i[0];
  byte oct2 = i[1];
  byte oct3 = i[2];
  byte oct4 = i[3];

  sprintf(str, "%d.%d.%d.%d", oct1, oct2, oct3, oct4);
  return str;
}

bool StatusPage::render() {
  Page::render();
  update();
  return true;
}

void StatusPage::selectButton(int clicks) {
  if (clicks == 2) {
    debugPrintf(DEBUG_TRACE, "Changing mode");
    nextMode();
  }
}

bool StatusPage::update() {
  display.setTop();
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "Running:    %s",
                  TimeToString(millis() / 1000));

  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "Mode:       %s", getCurrentModeName());

  if (wifiCurrentState == WIFI_CONNECTED) {
    display.println(DISPLAY_WHITE, DISPLAY_BLACK, "WiFi:       %s", WiFi.SSID());
    display.println(DISPLAY_WHITE, DISPLAY_BLACK, "    %s",         IPAddressToString(WiFi.localIP()));
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


  return true;
}

LightPage::LightPage() : Page("Lights") {
  // When constructed, we point to the first light
  _itspLight = allLights.begin();
}

bool LightPage::render() {
  // Assume that we either were already on this page, or someone else called display.clear
  Page::render();
  return true;
}

bool LightPage::update() {
  display.setTop();

  const HSIColor color = (*_itspLight)->getColor();
  display.println(DISPLAY_CYAN,  DISPLAY_BLACK, "Light: %s",            (*_itspLight)->getName());
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, " Temp: %4.4f",         (*_itspLight)->getTemperature());
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, " Local bright: %2.0f", (*_itspLight)->getLocalBrightness());
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, " E131 Addr: %u",       (*_itspLight)->getE131LocalAddress());
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "%s",                   (*_itspLight)->getDiagnostic());
  display.println(DISPLAY_WHITE,
                  DISPLAY_BLACK,
                  " C: %2.1f, %2.2f, %2.2f",
                  color.getHue(),
                  color.getSaturation(),
                  color.getIntensity());
  std::vector<outputChannel> o = (*_itspLight)->getOutputChannels();

  for (const auto& it : o) {
    if (it.power > 0.0f) {
      display.println(DISPLAY_WHITE, DISPLAY_BLACK, " %u : %2.2f", it.outputLocalAddress, it.power);
    }
  }
  return true;
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
  _itspLight--;
  render();
}

void LightPage::selectButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "LightPage::selectButton (%i)", clicks);
}
