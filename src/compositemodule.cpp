// *********************************************************

//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

// CompositeModule models a light that contains multiple emitters.
//
// After constructing, add white emitter using addWhiteEmitter and
// add color emitters using addColorEmitter.

#include "compositemodule.h"

#include "emitter.h"
#include "hsicolor.h"
#include "debug.h"

#include "application.h"
#include <vector>
#include <math.h>

CompositeModule::CompositeModule()
{}

void CompositeModule::addWhiteEmitter(const Emitter& white, uint16_t localAddress) {
  // slope and angle not relevant for white emitter
  // currently supports only one white emitter per composite module
  _whiteEmitter = componentEmitter(&white, localAddress, 0.0f, 0.0f);
  debugPrintf(DEBUG_INSANE, "Added white emitter %s at la %u", white.getName(), localAddress);
}

void CompositeModule::addColorEmitter(const Emitter& emitter, uint16_t localAddress) {
  // To figure out where to put it in the colorspace, calculate the angle from
  // the white point.
  float uEmitter = emitter.getU();
  float vEmitter = emitter.getV();
  float uWHITE   = _whiteEmitter.emitter->getU();
  float vWHITE   = _whiteEmitter.emitter->getV();

  debugPrintf(DEBUG_TRACE, "CompositeModule::addColorEmitter %s at address %u", emitter.getName(), localAddress);

  float newAngle = fmod((180 / M_PI) * atan2((vEmitter - vWHITE), (uEmitter - uWHITE)) + 360, 360);

  if (_colorEmitters.empty()) {
    // If it is the first LED, simply place it in the array.
    // With only one LED, slope is undefined.
    _colorEmitters.push_back(std::make_shared<componentEmitter>(componentEmitter(&emitter, localAddress, newAngle, 0)));
    debugPrintf(DEBUG_INSANE, "Added emitter %s at la %u", emitter.getName(), localAddress);
    return;
  }

  // Otherwise, place the LED at the appropriate point in the array, and also
  // recalculate slopes.

  // Create our component emitter based on the prototype emitter, plus this
  // one's particular address
  // TODO: This doesn't need to be a shared pointer. Only this class will ever use it.
  std::shared_ptr<componentEmitter> newEmitter =
    std::make_shared<componentEmitter>(componentEmitter(&emitter, localAddress, newAngle, 0));

  // Iterate through until finding the first location where the angle is bigger
  // than the current value.
  // TODO: Fix to use iterator
  std::vector<std::shared_ptr<componentEmitter> >::iterator it = _colorEmitters.begin();

  while ((it < _colorEmitters.end()) && ((*it)->angle < newAngle)) {
    it++;
  }

  // We will now either be in the correct slot in the vector, or at the end of
  // the vector
  // Insert the new emitter, set the pwm offset, set slope to zero pending
  // recalc
  _colorEmitters.insert(it, newEmitter);

  debugPrintf(DEBUG_INSANE, "Added emitter %s at la %u", emitter.getName(), localAddress);

  // And then recalculate all slopes
  for (std::vector<std::shared_ptr<componentEmitter> >::iterator itThisEmitter = _colorEmitters.begin();
       itThisEmitter < _colorEmitters.end();
       ++itThisEmitter) {
    std::shared_ptr<componentEmitter> spNextEmitter;

    // Slope is to the next emitter, except the last one wraps around to the
    // first
    if (*itThisEmitter != _colorEmitters.back()) {
      spNextEmitter = *(std::next(itThisEmitter));
    } else {
      spNextEmitter = *(_colorEmitters.begin());
    }

    // I hate this mix of iterators to shared pointers and shared pointers,
    // but it works
    (*itThisEmitter)->slope = (spNextEmitter->emitter->getV() - (*itThisEmitter)->emitter->getV())
                              / (spNextEmitter->emitter->getU() - (*itThisEmitter)->emitter->getU());
  }
}

float CompositeModule::getAngle(int num) {
  return _colorEmitters[num]->angle;
}

std::vector<outputEmitter>CompositeModule::Hue2EmitterPower(const HSIColor& HSI) const {
  float H = fmod(HSI.getHue() + 360, 360);
  float S = HSI.getSaturation();
  float I = HSI.getIntensity();

  float tanH = tan(M_PI * fmod(H, 360) / (float)180); // Get the tangent since
                                                      // we will use it often.

  // Copy our color emitters with default power of zero
  std::vector<outputEmitter> emitterPowers;

  for (std::vector<std::shared_ptr<componentEmitter> >::const_iterator itspEmitter = _colorEmitters.begin();
       itspEmitter < _colorEmitters.end();
       ++itspEmitter) {
    outputEmitter o((*itspEmitter)->localAddress, 0.0f);

    emitterPowers.push_back(o);
    debugPrintf(DEBUG_INSANE, "CompositeModule::Hue2EmitterPower added emitterPower[%u] at la %u with power %f",
                emitterPowers.size() - 1, emitterPowers.back().localAddress,
                emitterPowers.back().power);
  }

  unsigned int emitter1, emitter2;

  // Check the range to determine which intersection to do.
  // For angle less than the smallest CIE hue or larger than the largest,
  // special case.

  if ((H < _colorEmitters[0]->angle) || (H >= _colorEmitters[_colorEmitters.size() - 1]->angle)) {
    // Then we're mixing the lowest angle LED with the highest angle LED.
    emitter1 = _colorEmitters.size() - 1;
    emitter2 = 0;
  } else {
    // Iterate through the angles until we find an LED with hue smaller than the
    // angle.
    unsigned int i;

    for (i = 1; (H > _colorEmitters[i]->angle) && (i < (_colorEmitters.size() - 1)); i++) {
      if (H > _colorEmitters[i]->angle) {
        emitter1 = i - 1;
        emitter2 = i;
      }
    }

    // TODO: This seems redundant; is the whole "if" block ablove required?
    emitter1 = i - 1;
    emitter2 = i;
  }

  debugPrintf(DEBUG_INSANE, "Emitter 1: %u, Emitter 2: %u", emitter1, emitter2);

  // Get the ustar and vstar values for the target LEDs.
  float emitter1_ustar = _colorEmitters[emitter1]->emitter->getU() - _whiteEmitter.emitter->getU();
  float emitter1_vstar = _colorEmitters[emitter1]->emitter->getV() - _whiteEmitter.emitter->getV();
  float emitter2_ustar = _colorEmitters[emitter2]->emitter->getU() - _whiteEmitter.emitter->getU();
  float emitter2_vstar = _colorEmitters[emitter2]->emitter->getV() - _whiteEmitter.emitter->getV();

  debugPrintf(DEBUG_INSANE,
              "t: %f, e1u: %f, e1v: %f, e2u: %f, e2v: %f",
              tanH,
              emitter1_ustar,
              emitter1_vstar,
              emitter2_ustar,
              emitter2_vstar);

  // Get the slope between LED1 and LED2.
  float slope = _colorEmitters[emitter1]->slope;

  float ustar = (emitter2_vstar - slope * emitter2_ustar) / (tanH - slope);
  float vstar = tanH / (slope - tanH) * (slope * emitter2_ustar - emitter2_vstar);

  debugPrintf(DEBUG_INSANE, "s: %f, u: %f, v: %f", slope, ustar, vstar);

  // Set the two selected colors.
  emitterPowers[emitter1].power = I * S * abs(ustar - emitter2_ustar) / abs(emitter2_ustar - emitter1_ustar);
  emitterPowers[emitter2].power = I * S * abs(ustar - emitter1_ustar) / abs(emitter2_ustar - emitter1_ustar);

  debugPrintf(DEBUG_INSANE, "I: %3.2f S: %3.2f u: %3.2f v: %3.2f p1: %.4f p2: %.4f", I, S,
              ustar, vstar, emitterPowers.at(emitter1).power, emitterPowers.at(emitter2).power);

  // Add white to the end, and set the power
  emitterPowers.push_back(outputEmitter(_whiteEmitter.localAddress, I * (1 - S)));

  //  // For debugging, print the actual output values.
  //  Serial.println("Target Hue of " + String(H) + " between LEDs " +
  // String(LED1) + " and " + String(LED2));
  //  Serial.println("Output Values");
  //  for (std::vector<float>::iterator i=LEDOutputs.begin(); i !=
  // LEDOutputs.end(); ++i) {
  //    Serial.print(*i, 2);
  //    Serial.print(" ");
  //  }
  //  Serial.println("");

  return emitterPowers;
}
