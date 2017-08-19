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
#include "menus.h"
#include "luminous-particle.h"

Page* SetupMenus() {
  Menu topMenu = new Menu("Luminous", nullptr);

  StatusPage status = new StatusPage("Status");

  topMenu->addChild(&status);

  Menu lightList = new Menu("Lights", &topMenu);

  for (const auto& it : allLights) {
    LightPage lightPage = new LightPage(*it);
    lightList.addChild(&lightPage);
  }
  topMenu->addChild(&lightList);

  E131Page e131Page = new E131Page("Monitor");
  topMenu->addChild(&E131Page);

  return &topMenu;
}
