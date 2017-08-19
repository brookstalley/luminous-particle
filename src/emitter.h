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


#ifndef EMITTER_H
#define EMITTER_H

#include "Particle.h"

class Emitter {
private:

  float _u, _v;
  uint16_t _maxLumens;
  const char *_name;

public:

  Emitter(const char *name,
          float       u,
          float       v,
          uint16_t    maxLumens);
  float       getU(void) const;
  float       getV(void) const;
  uint16_t    getMaxLumens(void) const;
  const char* getName(void) const;
};
#endif /* ifndef EMITTER_H */
