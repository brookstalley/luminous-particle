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
  // To figure out where to put it in the colorspace, calculate the angle from
  // the white point.
  float uEmitter = emitter.getU();
  float vEmitter = emitter.getV();
  float uWHITE   = _whiteEmitter.emitter->getU();
  float vWHITE   = _whiteEmitter.emitter->getV();

  debugPrintf(DEBUG_TRACE,
              "CompositeModule::addColorEmitter %s at address %u",
              emitter.getName(),
              outputLocalAddress);

  float newAngle =
    fmod((180 / M_PI) * atan2((vEmitter - vWHITE), (uEmitter - uWHITE)) + 360,
         360);

  // Create our component emitter based on the prototype emitter, plus this
  // one's particular address
  std::shared_ptr<componentEmitter> newEmitter =
    std::make_shared<componentEmitter>(componentEmitter(&emitter,
                                                        outputLocalAddress,
                                                        newAngle, 0, effectOnly));

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

  // Sort the emitters by angle
  std::sort(_colorEmitters.begin(), _colorEmitters.end(),
            [](std::shared_ptr<componentEmitter>a,
               std::shared_ptr<componentEmitter>b) {
    return b->angle > a->angle;
  });

  // Recalculate all slopes and ustar / vtar
  for (auto it = _colorEmitters.begin(); it != _colorEmitters.end(); it++) {
    (*it)->ustar = (*it)->emitter->getU() - _whiteEmitter.emitter->getU();
    (*it)->vstar = (*it)->emitter->getV() - _whiteEmitter.emitter->getV();

    std::shared_ptr<componentEmitter> spNextEmitter;

    // Slope is to the next emitter, except the last one wraps around to the
    // first
    if (*it != _colorEmitters.back()) {
      spNextEmitter = *(std::next(it));
    } else {
      spNextEmitter = _colorEmitters.front();
    }

    // I hate this mix of iterators to shared pointers and shared pointers,
    // but it works
    (*it)->slope = (spNextEmitter->emitter->getV() - (*it)->emitter->getV())
                   / (spNextEmitter->emitter->getU() - (*it)->emitter->getU());
  }

  // Build wedge list (experimental)
  _colorspaceWedges.clear();

  for (auto it = _colorEmitters.begin(); it != _colorEmitters.end(); it++) {
    if (!(*it)->effectOnly) {
      bool found  = false;
      auto itNext = it;

      while (!found) {
        if (*itNext != _colorEmitters.back()) {
          *itNext++;
        } else {
          *itNext = _colorEmitters.front();
        }

        if (!(*itNext)->effectOnly) {
          found = true;
        }
      }

      // Now it points to the start of the wedge and itNext to the end
      float slope = ((*itNext)->emitter->getV() - (*it)->emitter->getV())
                    / ((*itNext)->emitter->getU() - (*it)->emitter->getU());
      float e1ustar = (*it)->emitter->getU() - _whiteEmitter.emitter->getU();
      float e1vstar = (*it)->vstar = (*it)->emitter->getV() -
                                     _whiteEmitter.emitter->getV();
      float e2ustar = (*itNext)->emitter->getU() - _whiteEmitter.emitter->getU();
      float e2vstar = (*itNext)->vstar = (*itNext)->emitter->getV() -
                                         _whiteEmitter.emitter->getV();

      colorspaceWedge wedge((*it)->angle,
                            (*itNext)->angle,
                            slope,
                            (*it)->emitter,
                            e1ustar,
                            e1vstar,
                            (*itNext)->emitter,
                            e2ustar,
                            e2vstar);
      _colorspaceWedges.push_back(wedge);
    }
  }

  for (const auto& e : _colorEmitters) {
    debugPrintf(DEBUG_TRACE, "Emitter %s: angle %4.4f slope %4.4f",
                e->emitter->getName(),
                e->angle,
                e->slope);
  }

  for (const auto& w : _colorspaceWedges) {
    debugPrintf(DEBUG_TRACE, "Wedge: start %4.4f(%s), end %4.4f(%s), slope ",
                w.startAngle,
                w.emitter1->getName(),
                w.endAngle,
                w.emitter2->getName(),
                w.slope);
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

  // Get the slope between LED1 and LED2.
  float slope = emitter1->slope;

  float ustar = (emitter2->vstar - slope * emitter2->ustar) / (tanH - slope);
  float vstar = tanH / (slope - tanH) *
                (slope * emitter2->ustar - emitter2->vstar);

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

std::vector<outputEmitter>CompositeModule::emitterPowersFromHSI2(
  const HSIColor& HSI) const {
  /***** Experimental version using colorspacewedges ******/

  float H = fmod(HSI.getHue() + 360, 360);
  float S = HSI.getSaturation();
  float I = HSI.getIntensity();

  float tanH = tan(M_PI * fmod(H, 360) / (float)180); // Get the tangent since

  // we will use it often.

  std::shared_ptr<componentEmitter> emitter1;
  std::shared_ptr<componentEmitter> emitter2;

  // We're looking for the wedge that the hue angle lives in
  std::vector<colorspaceWedge>::const_iterator it =
    std::find_if(_colorspaceWedges.begin(), _colorspaceWedges.end(),
                 [H](const colorspaceWedge w) -> bool {
    // We found the right wedge if 1) startAngle <= H <= endAngle, 2) startAngle
    // < H && endAngle < startAngle (H is between the start of the last wedge
    // and 0)
    return (w.startAngle <= H) &&
    ((w.endAngle >= H) || (w.endAngle < w.startAngle));
  });

  // ...or 3) 0 < H < wedge[0].startAngle (in which case we're in the last
  // wedge, since it wraps around)
  if (it == _colorspaceWedges.end()) {
    it = _colorspaceWedges.end() - 1;
  }

  colorspaceWedge wedge = *it;

  debugPrintf(DEBUG_INSANE,
              "Hue %f: Emitter 1: %s (%4.0f), Emitter 2: %s (%4.0f)",
              H,
              wedge.emitter1->getName(),
              wedge.startAngle,
              wedge.emitter2->getName(),
              wedge.endAngle);

  float ustar = (wedge.e2vstar - wedge.slope * wedge.e2ustar) /
                (tanH - wedge.slope);
  float vstar = tanH / (wedge.slope - tanH) *
                (wedge.slope * wedge.e2ustar - wedge.e2vstar);

  debugPrintf(DEBUG_INSANE, "s: %f, u: %f, v: %f", wedge.slope, ustar, vstar);

  // Calculate separately because abs() is a goddamned macro that returns
  // goddamned integers which goddamned breaks
  // TODO: Should scale each emitter relative to the other's maximum power
  float emitter1power = I * S * fabs(ustar - wedge.e2ustar) / fabs(
    wedge.e2ustar - wedge.e1ustar);
  float emitter2power = I * S * fabs(ustar - wedge.e1ustar) / fabs(
    wedge.e2ustar - wedge.e1ustar);

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
    if ((*itspEmitter)->emitter == wedge.emitter1) {
      emitterPower = emitter1power;
    } else if ((*itspEmitter)->emitter == wedge.emitter2) {
      emitterPower = emitter2power;
    } else {
      emitterPower = 0.0f;
    }
    outputEmitter o((*itspEmitter)->emitter, (*itspEmitter)->outputLocalAddress, emitterPower);
    emitterPowers.push_back(o);
  }

  // Add white to the end, and set the power based on saturation
  emitterPowers.push_back(outputEmitter(_whiteEmitter.emitter, _whiteEmitter.outputLocalAddress,
                                        I * (1 - S)));

  if ((emitter1power > 1.0f) || (emitter2power > 1.0f)) {
    debugPrintf(DEBUG_ERROR,
                "Power>1: t: %f, e1u: %f, e2u: %f, e2v: %f I: %3.2f S: %3.2f u: %f v: %f p1: %f p2: %f",
                tanH,
                wedge.e1ustar,
                wedge.e2ustar,
                wedge.e2vstar,
                I,
                S,
                ustar,
                vstar,
                emitter1power,
                emitter2power);
  }

  return emitterPowers;
}
