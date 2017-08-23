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
#include "pages.h"
#include "menu.h"
#include "menus.h"
#include "luminous-particle.h"

#include <utility>

std::vector<std::shared_ptr<Page> > pageStack;

bool setupMenus() {
  std::shared_ptr<Page> topMenu = std::make_shared<Menu>("Luminous");
  std::shared_ptr<Page> status  = std::make_shared<StatusPage>();

  std::shared_ptr<Page> lightList = std::make_shared<LightPage>();

  topMenu->addChild(lightList);
  topMenu->addChild(statusMenu);

  pageStack.push_front(topMenu);
  return true;
}

bool menuPop() {
  if (pageStack.begin() != pageStack.back()) {
    menuStack.pop_front();
    return true;
  }
  return false;
}

bool redrawCurrentPage() {
  pageStack.at(0).render();
}