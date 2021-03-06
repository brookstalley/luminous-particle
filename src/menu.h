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
  std::vector<std::shared_ptr<Page> >_childItems;
  std::vector<std::shared_ptr<Page> >::iterator _selectedItem;
  bool render();
  bool update();
  bool moveNext();
  bool movePrev();
  bool selectItem();

public:

  Menu(const char *name);
  void addChild(const std::shared_ptr<Page>);

  void backButton(int clicks);
  void nextButton(int clicks);
  void prevButton(int clicks);
  void selectButton(int clicks);
};

#endif // ifndef LUMINOUS_MENU_H
