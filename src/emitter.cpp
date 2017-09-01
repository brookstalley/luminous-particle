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


#include "emitter.h"
#include "color.h"
#include "Particle.h"
#include <memory>

Emitter::Emitter(const char *name, std::shared_ptr<cieUVcolor>uv, uint16_t maxLumens)
{
  _name      = name;
  _uv        = uv;
  _maxLumens = maxLumens;
}

Emitter::Emitter(const char *name, uint16_t wavelength, uint16_t maxLumens)
{
  _name = name;
  auto uv = std::make_shared<cieUVcolor>(wavelength);
  _uv        = uv;
  _maxLumens = maxLumens;
}

std::shared_ptr<cieUVcolor>Emitter::getUV(void) const {
  return _uv;
}

uint16_t Emitter::getMaxLumens(void) const {
  return _maxLumens;
}

const char * Emitter::getName(void) const {
  return _name;
}
