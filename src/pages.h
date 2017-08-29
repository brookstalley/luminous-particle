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

#ifndef LUMINOUS_PAGES_H
#define LUMINOUS_PAGES_H

#include "page.h"
#include "hsilight.h"
#include <vector>
#include <memory>

// Each page is a derived class from the main page
class StatusPage : public Page {
public:

  StatusPage() : Page("Status") {}

  bool render();
  bool update();
  void selectButton(int clicks) override;
};

class LightPage : public Page {
private:

  // Just calling it light because at some point we'll generalize lights to not always be HSI
  std::vector<std::shared_ptr<HSILight> >::iterator _itspLight;

public:

  LightPage();

  bool render();
  bool update();
  void nextButton(int clicks);
  void prevButton(int clicks);
  void selectButton(int clicks);
};

char * IPAddressToString(IPAddress i);

#endif // ifndef LUMINOUS_PAGES_H
