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
  _whiteEmitter = componentEmitter(&white, outputLocalAddress, false);
  debugPrintf(DEBUG_INSANE,
              "Added white emitter %s at la %u",
              white.getName(), outputLocalAddress);
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
                                                        outputLocalAddress, effectOnly));

  // Note that we add the emitter at the end without regard for order
  // CompositeModule WILL NOT WORK unless calculate() is called after adding
  // last emitter
  _colorEmitters.push_back(newEmitter);

  debugPrintf(DEBUG_INSANE,
              "Added emitter %s at la %u", emitter.getName(), outputLocalAddress);
}

void CompositeModule::calculate() {
  debugPrint(DEBUG_TRACE, "CompositeModule::calculate start");

  auto whiteUV = *(_whiteEmitter.emitter->getUV());

  // Recalculate all slopes, ustar / vstar, and angle to white
  for (auto it = _colorEmitters.begin(); it != _colorEmitters.end(); it++) {
    auto colorUV = *((*it)->emitter->getUV());
    (*it)->ustar = colorUV.u - whiteUV.u;
    (*it)->vstar = colorUV.v - whiteUV.v;
    (*it)->angle =
      fmod((180 / M_PI) * atan2((colorUV.v - whiteUV.v), (colorUV.u - whiteUV.u)) + 360, 360);

    std::shared_ptr<componentEmitter> spNextEmitter;
  }

  // Sort the emitters by angle
  std::sort(_colorEmitters.begin(), _colorEmitters.end(),
            [](std::shared_ptr<componentEmitter>a, std::shared_ptr<componentEmitter>b) {
    return b->angle > a->angle;
  });

  // Now get the slope to the next emitter
  for (auto it = _colorEmitters.begin(); it != _colorEmitters.end(); it++) {
    // Slope is to the next emitter, except the last one wraps around to the
    // first
    auto itNext = ((it + 1) != _colorEmitters.end()) ? std::next(it) : _colorEmitters.begin();

    auto thisUV = *((*it)->emitter->getUV());
    auto nextUV = *((*itNext)->emitter->getUV());

    (*it)->slope = (nextUV.v - thisUV.v) / (nextUV.u - thisUV.u);
  }

  for (const auto& e : _colorEmitters) {
    debugPrintf(DEBUG_TRACE, "Emitter %s: u %3.2f v %3.2f angle %4.4f slope %4.4f",
                e->emitter->getName(), e->emitter->getUV()->u, e->emitter->getUV()->v, e->angle, e->slope);
  }
}

std::vector<outputChannel>CompositeModule::emitterPowersFromHSI(
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
    std::find_if(_colorEmitters.begin(), _colorEmitters.end(),
                 [H](const std::shared_ptr<componentEmitter>e) -> bool {
    return e->angle > H;
  });

  if ((it == _colorEmitters.end()) || (it == _colorEmitters.begin())) {
    emitter1 = _colorEmitters.back();
    emitter2 = _colorEmitters.front();
  } else {
    emitter1 = *(it - 1);
    emitter2 = *it;
  }

  float ustar = (emitter2->vstar - emitter1->slope * emitter2->ustar) / (tanH - emitter1->slope);
  float vstar = tanH / (emitter1->slope - tanH) *
                (emitter1->slope * emitter2->ustar - emitter2->vstar);

  // Calculate separately because abs() is a goddamned macro that returns
  // goddamned integers which goddamned breaks
  float emitter1power = I * S * fabs(ustar - emitter2->ustar) / fabs(
    emitter2->ustar - emitter1->ustar);
  float emitter2power = I * S * fabs(ustar - emitter1->ustar) / fabs(
    emitter2->ustar - emitter1->ustar);

  // Copy our color emitters with default power of zero
  std::vector<outputChannel> outputChannels;
  float emitterPower;

  // Fill our output power list. For each component emitter, set it to zero
  // unless it is one of the two emitters we are using to mix the color. If it's
  // one of them, set the appropriate power.
  for (auto itspEmitter = _colorEmitters.begin(); itspEmitter < _colorEmitters.end(); ++itspEmitter) {
    if ((*itspEmitter) == emitter1) {
      emitterPower = emitter1power;
    } else if ((*itspEmitter) == emitter2) {
      emitterPower = emitter2power;
    } else {
      emitterPower = 0.0f;
    }
    outputChannel o((*itspEmitter)->outputLocalAddress, emitterPower);

    outputChannels.push_back(o);
  }

  // Add white to the end, and set the power based on saturation
  outputChannels.push_back(outputChannel(_whiteEmitter.outputLocalAddress, I * (1 - S)));

  debugPrintf(DEBUG_TRACE,
              "HSI: %3f %3.2f %3.2f | t: %3.2f | e1: %s (%3.2f), e2: %s (%3.2f) | e1s: %3.2f e1u: %f, e1v: %f, e2u: %f, e2v: %f | u: %f v: %f | p1: %f p2: %f pI: %f",
              H,
              S,
              I,
              tanH,
              emitter1->emitter->getName(),
              emitter1->angle,
              emitter2->emitter->getName(),
              emitter2->angle,
              emitter1->slope,
              emitter1->ustar,
              emitter1->vstar,
              emitter2->ustar,
              emitter2->vstar,
              ustar,
              vstar,
              emitter1power,
              emitter2power,
              I * (1 - S));

  return outputChannels;
}
