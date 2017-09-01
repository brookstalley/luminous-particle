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
#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include "emitter.h"

#include <vector>

class OutputInterface {
public:

  virtual bool setEmitterPowers(const std::vector<outputChannel>& channels,
                                float                             scaleFactor) = 0;

  virtual bool init()         = 0;
  virtual bool sleep() const  = 0;
  virtual bool allOff() const = 0;
};

#endif /* ifndef OUTPUTINTERFACE_H */
