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

#include "page.h"
#include "display.h"
#include "ldebug.h"
#include "luminous-particle.h"

Page::Page(const char *name) :
  _name(name)
{}

bool Page::render() {
  debugPrintf(DEBUG_TRACE, "Page::render begin (%s)", _name);
  display.clear();
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, _name);
  return true;
}

bool Page::update() {
  debugPrintf(DEBUG_TRACE, "Page::update begin (%s)", _name);
  display.println(DISPLAY_WHITE, DISPLAY_BLACK, _name);
  return true;
}

const char * Page::getName() {
  return _name;
}

void Page::setName(const char *name) {
  _name = name;
}

void Page::backButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "Page::backButton (%i)", clicks);
}

void Page::nextButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "Page::nextButton (%i)", clicks);
}

void Page::prevButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "Page::prevButton (%i)", clicks);
}

void Page::selectButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "Page::selectButton (%i)", clicks);
}
