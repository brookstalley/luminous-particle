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

#ifndef COMPOSITEMODULE_H
#define COMPOSITEMODULE_H

#include "emitter.h"
#include "hsicolor.h"

#include "Particle.h"

#include <vector>
#include <algorithm>

struct outputEmitter {
  outputEmitter(const Emitter *e, uint8_t la, float pwr) {
    emitter            = e;
    outputLocalAddress = la;
    power              = pwr;
  }

  const Emitter *emitter;
  uint16_t       outputLocalAddress;
  float          power;
};


struct componentEmitter {
  componentEmitter(const Emitter *e, uint16_t la, float a, float s, bool eo) {
    emitter            = e;
    outputLocalAddress = la;
    angle              = a;
    slope              = s;
    effectOnly         = eo;
    ustar              = 0;
    vstar              = 0;
  }

  componentEmitter() {}

  const Emitter *emitter;
  uint16_t       outputLocalAddress;
  float          angle; // from white point
  float          slope; // to next componentEmitter; last one is slope to the
                        // first
  bool  effectOnly;
  float ustar;
  float vstar;
};

class CompositeModule {
private:

  std::vector<std::shared_ptr<componentEmitter> >_colorEmitters;
  componentEmitter _whiteEmitter;
  float _dimTemperature;
  float _shutdownTemperature;

public:

  CompositeModule(float dimTemperature,
                  float shutdownTemperature);
  void                      addWhiteEmitter(const Emitter& white,
                                            uint16_t       outputLocalAddress);
  void                      addColorEmitter(const Emitter& emitter,
                                            uint16_t       outputLocalAddress,
                                            bool           effectOnly);
  void                      calculate();
  std::vector<outputEmitter>emitterPowersFromHSI(const HSIColor& HSI) const;
};

#endif /* ifndef COMPOSITEMODULE_H */
