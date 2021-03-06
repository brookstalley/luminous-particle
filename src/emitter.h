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

#include "color.h"
#include "Particle.h"
#include <memory>

class Emitter {
private:

  std::shared_ptr<cieUVcolor>_uv;
  uint16_t _maxLumens;
  const char *_name;

public:

  Emitter(const char                *name,
          std::shared_ptr<cieUVcolor>uv,
          uint16_t                   maxLumens);

  Emitter(const char *name,
          uint16_t    wavelength,
          uint16_t    maxLumens);

  std::shared_ptr<cieUVcolor>getUV(void) const;
  uint16_t                   getMaxLumens(void) const;
  const char               * getName(void) const;
};
#endif /* ifndef EMITTER_H */
