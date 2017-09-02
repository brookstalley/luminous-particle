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
#ifndef MODES_H
#define MODES_H

#include "luminous-particle.h"

#include "mode.h"
#include "hsilight.h"
#include "hsicolor.h"
#include "ldebug.h"

#include "Particle.h"

#include <vector>
#include <memory>

class ModeOff : public Mode {
public:

  using Mode::Mode;
  bool start(std::vector<std::shared_ptr<HSILight> >);
};

class ModeTest : public Mode {
public:

  using Mode::Mode;
  bool run(std::vector<std::shared_ptr<HSILight> >,
           bool);
};

class ModeRotate : public Mode {
private:

  HSIColor _rotateColor;

public:

  using Mode::Mode;

  bool doRotate();
  bool start(std::vector<std::shared_ptr<HSILight> >);
  bool run(std::vector<std::shared_ptr<HSILight> >,
           bool);
};

class ModeE131 : public Mode {
public:

  using Mode::Mode;
  bool       start(std::vector<std::shared_ptr<HSILight> >);
  bool       run(std::vector<std::shared_ptr<HSILight> >,
                 bool);
  bool       end(std::vector<std::shared_ptr<HSILight> >);
};

void         nextMode();
bool         setModeByNumber(uint16_t modeNumber);
bool         setModeByName(String modeName);
void         runCurrentMode();
const String getCurrentModeName();

// Stray, but this seems a reasonable place for it
float        twoBytesToFloat(const uint8_t *buf);


#endif /* ifndef MODES_H */
