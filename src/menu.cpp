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
#include "menu.h"
#include "ldebug.h"
#include "luminous-particle.h"

Menu::Menu(const char *name) : Page(name) {
  _selectedItem = _childItems.begin();
}

void Menu::addChild(const std::shared_ptr<Page>childItem) {
  _childItems.push_back(childItem);
  _selectedItem = _childItems.begin();
}

bool Menu::moveNext() {
  _selectedItem++;

  if (_selectedItem == _childItems.end()) {
    _selectedItem = _childItems.begin();
  }
  return true;
}

bool Menu::movePrev() {
  if (_selectedItem == _childItems.begin()) {
    _selectedItem = _childItems.end();
  }
  _selectedItem--;
  return true;
}

bool Menu::render() {
  // Whatever we're doing for all pages
  debugPrintf(DEBUG_TRACE, "Menu::render starting");
  Page::render();

  for (auto& itp : _childItems) {
    if (itp != *_selectedItem) {
      display.println(DISPLAY_WHITE, DISPLAY_BLACK, (*itp).getName());
    }
    else {
      display.println(DISPLAY_BLACK, DISPLAY_WHITE, (*itp).getName());
    }
  }
  return true;
}

bool Menu::update() {
  return true;
}

void Menu::nextButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "Menu::nextButton (%i)", clicks);
  moveNext();
  render();
}

void Menu::prevButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "Menu::prevButton (%i)", clicks);
  movePrev();
  render();
}

void Menu::selectButton(int clicks) {
  debugPrintf(DEBUG_TRACE, "Menu::selectButton (%i)", clicks);
  pageStack.push_back(*_selectedItem);
  pageStack.back()->render();
}
