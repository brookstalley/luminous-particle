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

// CompositeModule models a light that contains multiple emitters.
//
// After constructing, add white emitter using addWhiteEmitter and
// add color emitters using addColorEmitter.

#include "compositemodule.h"

#include "emitter.h"
#include "hsicolor.h"
#include "ldebug.h"

#include "Particle.h"

#include <vector>
#include <math.h>

CompositeModule::CompositeModule(float dimTemperature,
                                 float shutdownTemperature) :
  _dimTemperature(dimTemperature),
  _shutdownTemperature(shutdownTemperature)
{}

void CompositeModule::addWhiteEmitter(const Emitter& white,
                                      uint16_t       outputLocalAddress) {
  // slope and angle not relevant for white emitter
  // currently supports only one white emitter per composite module
  _whiteEmitter = componentEmitter(&white, outputLocalAddress, 0.0f, 0.0f, false);
  debugPrintf(DEBUG_INSANE,
              "Added white emitter %s at la %u",
              white.getName(),
              outputLocalAddress);
}

void CompositeModule::addColorEmitter(const Emitter& emitter,
                                      uint16_t       outputLocalAddress,
                                      bool           effectOnly) {
  debugPrintf(DEBUG_TRACE,
              "CompositeModule::addColorEmitter %s at address %u",
              emitter.getName(),
              outputLocalAddress);

  // Create our component emitter based on the prototype emitter, plus this
  // one's particular address
  std::shared_ptr<componentEmitter> newEmitter =
    std::make_shared<componentEmitter>(componentEmitter(&emitter,
                                                        outputLocalAddress,
                                                        0, 0, effectOnly));

  // Note that we add the emitter at the end without regard for order
  // CompositeModule WILL NOT WORK unless calculate() is called after adding
  // last emitter
  _colorEmitters.push_back(newEmitter);

  debugPrintf(DEBUG_INSANE,
              "Added emitter %s at la %u",
              emitter.getName(),
              outputLocalAddress);
}

void CompositeModule::calculate() {
  debugPrint(DEBUG_TRACE,
             "CompositeModule::calculate start");

  cieUVcolor whiteUV = _whiteEmitter.emitter->getUV();

  // Recalculate all slopes, ustar / vstar, and angle to white
  for (auto it = _colorEmitters.begin(); it != _colorEmitters.end(); it++) {
    cieUVcolor colorUV = (*it)->emitter->getUV();
    (*it)->ustar = colorUV.u - whiteUV.u;
    (*it)->vstar = colorUV.v - whiteUV.v;
    (*it)->angle =
      fmod(atan2((colorUV.v - whiteUV.v), (colorUV.u - whiteUV.u)) + 360, 360);

    std::shared_ptr<componentEmitter> spNextEmitter;
  }

  // Sort the emitters by angle
  std::sort(_colorEmitters.begin(), _colorEmitters.end(),
            [](std::shared_ptr<componentEmitter>a,
               std::shared_ptr<componentEmitter>b) {
    return b->angle > a->angle;
  });

  // Now get the slope to the next emitter
  for (auto it = _colorEmitters.begin(); it != _colorEmitters.end(); it++) {
    // Slope is to the next emitter, except the last one wraps around to the
    // first
    if (*it != _colorEmitters.back()) {
      spNextEmitter = *(std::next(it));
    } else {
      spNextEmitter = _colorEmitters.front();
    }

    (*it)->slope = (spNextEmitter->emitter->getV() - colorUV.v)
                   / (spNextEmitter->emitter->getU() - colorUV.u);
  }

  for (const auto& e : _colorEmitters) {
    debugPrintf(DEBUG_TRACE, "Emitter %s: angle %4.4f slope %4.4f",
                e->emitter->getName(),
                e->angle,
                e->slope);
  }
}

float CompositeModule::getAngle(int num) {
  return _colorEmitters[num]->angle;
}

std::vector<outputEmitter>CompositeModule::emitterPowersFromHSI(
  const HSIColor& HSI) const {
  float H = fmod(HSI.getHue() + 360, 360);
  float S = HSI.getSaturation();
  float I = HSI.getIntensity();

  float tanH = tan(M_PI * fmod(H, 360) / (float)180); // Get the tangent since we will use it often.

  std::shared_ptr<componentEmitter> emitter1;
  std::shared_ptr<componentEmitter> emitter2;

  // We're looking for the emitters on either side of hue angle (H)
  // Start with the first one and walk up until we find an angle
  // bigger than H. H is bewtween that angle and the previous one
  // (component emitter list is sorted in ascending order)
  std::vector<std::shared_ptr<componentEmitter> >::const_iterator it =
    std::find_if(_colorEmitters.begin() + 1, _colorEmitters.end(),
                 [H](const std::shared_ptr<componentEmitter>e) -> bool {
    return e->angle > H;
  });

  if ((it == _colorEmitters.end()) || ((it + 1) == _colorEmitters.end())) {
    emitter1 = _colorEmitters.back();
    emitter2 = _colorEmitters.front();
  } else {
    emitter1 = *(it - 1);
    emitter2 = *it;
  }

  debugPrintf(DEBUG_INSANE,
              "Hue %f: Emitter 1: %s (%4.0f), Emitter 2: %s (%4.0f)",
              H,
              emitter1->emitter->getName(),
              emitter1->angle,
              emitter2->emitter->getName(),
              emitter2->angle);

  float ustar = (emitter2->vstar - emitter1->slope * emitter2->ustar) / (tanH - emitter1->slope);
  float vstar = tanH / (emitter1->slope - tanH) *
                (emitter1->slope * emitter2->ustar - emitter2->vstar);

  debugPrintf(DEBUG_INSANE, "s: %f, u: %f, v: %f", slope, ustar, vstar);

  // Calculate separately because abs() is a goddamned macro that returns
  // goddamned integers which goddamned breaks
  float emitter1power = I * S * fabs(ustar - emitter2->ustar) / fabs(
    emitter2->ustar - emitter1->ustar);
  float emitter2power = I * S * fabs(ustar - emitter1->ustar) / fabs(
    emitter2->ustar - emitter1->ustar);

  // Copy our color emitters with default power of zero
  std::vector<outputEmitter> emitterPowers;
  float emitterPower;

  // Fill our output power list. For each component emitter, set it to zero
  // unless it is one of the two emitters we are using to mix the color. If it's
  // one of them, set the appropriate power.
  for (std::vector<std::shared_ptr<componentEmitter> >::const_iterator itspEmitter
         = _colorEmitters.begin();
       itspEmitter < _colorEmitters.end();
       ++itspEmitter) {
    if ((*itspEmitter) == emitter1) {
      emitterPower = emitter1power;
    } else if ((*itspEmitter) == emitter2) {
      emitterPower = emitter2power;
    } else {
      emitterPower = 0.0f;
    }
    outputEmitter o((*itspEmitter)->emitter, (*itspEmitter)->outputLocalAddress, emitterPower);

    emitterPowers.push_back(o);

    /*
       debugPrintf(DEBUG_INSANE, "CompositeModule::emitterPowersFromHSI added
          emitterPower[%u] at la %u with power %f",
                emitterPowers.size() - 1,
                   emitterPowers.back().outputLocalAddress,
                emitterPowers.back().power);
     */
  }

  // Add white to the end, and set the power based on saturation
  emitterPowers.push_back(outputEmitter(_whiteEmitter.emitter, _whiteEmitter.outputLocalAddress,
                                        I * (1 - S)));

  if ((emitter1power > 1.0f) || (emitter2power > 1.0f)) {
    debugPrintf(DEBUG_INSANE,
                "t: %f, e1u: %f, e2u: %f, e2v: %f I: %3.2f S: %3.2f u: %f v: %f p1: %f p2: %f",
                tanH,
                emitter1->ustar,
                emitter2->ustar,
                emitter2->vstar,
                I,
                S,
                ustar,
                vstar,
                emitter1power,
                emitter2power);
  }

  return emitterPowers;
}
