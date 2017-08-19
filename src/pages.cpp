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
#include "luminous-particle.h"

bool StatusPage::render() {
  Page::render();
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "(status page)");
  return true;
}

LightPage::LightPage(const std::shared_ptr<HSILight>light, const Page *parent)
  : Page(light->getName(), parent), _light(light) {}

bool LightPage::render() {
  // Assume that we either were already on this page, or someone else called display.clear
  Page::render();
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, "E131 Address: %u", _light->getE131LocalAddress());
  return true;
}
