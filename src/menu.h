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

#ifndef LUMINOUS_MENU_H
#define LUMINOUS_MENU_H

#include "page.h"
#include <vector>

class Menu : public Page {
private:

  // childItems can be another menu, or any Page
  std::vector<Menu>_childItems;
  std::vector<Menu>::iterator _selectedItem;

public:

  Menu(const char *name);
  addChild(Page * childPage);

  bool moveSelection(schar direction);
  bool selectItem();
}

#endif // ifndef LUMINOUS_MENU_H
