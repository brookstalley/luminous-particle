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
#ifndef MODE_H
#define MODE_H

#include "hsilight.h"

#include <vector>
#include <memory>

class Mode {
private:

  String _name;

public:

  Mode(String name);
  virtual ~Mode() {}

  virtual bool start(std::vector<std::shared_ptr<HSILight> >lights);
  virtual bool run(std::vector<std::shared_ptr<HSILight> >lights,
                   bool                                   lightsMustUpdate);
  virtual bool end(std::vector<std::shared_ptr<HSILight> >lights);

  const String getName();
};

#endif /* ifndef MODE_H */
