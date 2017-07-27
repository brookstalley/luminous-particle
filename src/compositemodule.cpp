//*********************************************************
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

#include <math.h>

CompositeModule::CompositeModule ()
{

}

void CompositeModule::addWhiteEmitter(const Emitter &white, uint16_t localAddress) {
  // slope and angle not relevant for white emitter
  // currently supports only one white emitter per composite module
  _whiteEmitter = componentEmitter(&white, localAddress, 0.0f, 0.0f);
}

void CompositeModule::addColorEmitter (Emitter &emitter, uint16_t localAddress) {
  // To figure out where to put it in the colorspace, calculate the angle from the white point.
  float uEmitter = emitter.getU();
  float vEmitter = emitter.getV();
  float uWHITE = _whiteEmitter.emitter->getU();
  float vWHITE = _whiteEmitter.emitter->getV();

  float angle = fmod((180/M_PI) * atan2((vEmitter - vWHITE),(uEmitter - uWHITE)) + 360, 360);

  if (_colorEmitters.empty()) {
    // If it is the first LED, simply place it in the array.
    // With only one LED, slope is undefined.
    _colorEmitters.push_back(componentEmitter(&emitter, localAddress, angle, 0));
  } else {
    // Otherwise, place the LED at the appropriate point in the array, and also recalculate slopes.
    unsigned int insertlocation;

    // Iterate through until finding the first location where the angle is bigger than the current value.
    for (insertlocation = 0; (_colorEmitters[insertlocation].angle < angle) &&
        (insertlocation < _colorEmitters.size()); insertlocation++);

    // Insert the new emitter, set the pwm offset, set slope to zero pending recalc
    _colorEmitters.insert(_colorEmitters.begin() + insertlocation
      , componentEmitter(&emitter, localAddress, angle, 0));

    // And then recalculate all slopes except the last
    for (unsigned int i=0; i<_colorEmitters.size(); i++) {
      unsigned int nextEmitter;
      // Slope is to the next emitter, except the last one wraps around to the first
      if (i < _colorEmitters.size() -1) {
        nextEmitter = i+1;
      } else {
        nextEmitter = 0;
      }
      _colorEmitters[i].slope = (_colorEmitters[nextEmitter].emitter->getV() - _colorEmitters[i].emitter->getV())
        / (_colorEmitters[nextEmitter].emitter->getU() - _colorEmitters[i].emitter->getU());
    }
  }

//  // For debugging, print the current array of angles.
//  Serial.println("Current LED Angles");
//  for (std::vector<float>::iterator i=_angle.begin(); i != _angle.end(); ++i) {
//    Serial.println(*i, 5);
//  }
//  Serial.println("");
//
//  // For debugging, print the current array of slopes.
//  Serial.println("Current LED Slopes");
//  for (std::vector<float>::iterator i=_slope.begin(); i != _slope.end(); ++i) {
//    Serial.println(*i, 5);
//  }
//  Serial.println("");
}

float CompositeModule::getAngle(int num) {
  return _colorEmitters[num].angle;
}

std::vector<outputEmitter> CompositeModule::Hue2EmitterPower(const HSIColor &HSI) const {
  float H = fmod(HSI.getHue()+360,360);
  float S = HSI.getSaturation();
  float I = HSI.getIntensity();

  float tanH = tan(M_PI*fmod(H,360)/(float)180); // Get the tangent since we will use it often.

  // Copy our color emitters with default power of zero
  std::vector<outputEmitter> emitterPowers;
  for (unsigned int i = 0; i < _colorEmitters.size(); i++) {
    emitterPowers.push_back(outputEmitter(_colorEmitters[i].localAddress, 0.0f));
  }

  //debugPrint("Allocated emitterPowers");
  unsigned int emitter1, emitter2;

  //char m[100];
  //sprintf(m, "emitterPowers: %u", emitterPowers.size());
  //debugPrint(m);
  //sprintf(m, "_colorEmitters: %u", _colorEmitters.size());
  //debugPrint(m);

  // Check the range to determine which intersection to do.
  // For angle less than the smallest CIE hue or larger than the largest, special case.


  if ((H < _colorEmitters[0].angle) || (H >= _colorEmitters[_colorEmitters.size()-1].angle)) {
    // Then we're mixing the lowest angle LED with the highest angle LED.
    emitter1 = _colorEmitters.size() - 1;
    emitter2 = 0;
  } else {
    // Iterate through the angles until we find an LED with hue smaller than the angle.
    unsigned int i;
    for (i=1; (H > _colorEmitters[i].angle) && (i<(_colorEmitters.size()-1)); i++) {
      if (H > _colorEmitters[i].angle) {
        emitter1 = i-1;
        emitter2 = i;
      }
    }
    // TODO: This seems redundant; is the whole "if" block ablove required?
    emitter1 = i-1;
    emitter2 = i;
  }
  char msg[100];
  sprintf(msg, "Emitter 1: %u, Emitter 2: %u", emitter1, emitter2);
  debugPrint("Found emitters");
  debugPrint(msg);

  // Get the ustar and vstar values for the target LEDs.
  float emitter1_ustar = _colorEmitters[emitter1].emitter->getU() - _whiteEmitter.emitter->getU();
  float emitter1_vstar = _colorEmitters[emitter1].emitter->getV() - _whiteEmitter.emitter->getV();
  float emitter2_ustar = _colorEmitters[emitter2].emitter->getU() - _whiteEmitter.emitter->getU();
  float emitter2_vstar = _colorEmitters[emitter2].emitter->getV() - _whiteEmitter.emitter->getV();

  // Get the slope between LED1 and LED2.
  float slope = _colorEmitters[emitter1].slope;

  float ustar = (emitter2_vstar - slope*emitter2_ustar)/(tanH - slope);
  float vstar = tanH/(slope - tanH) * (slope * emitter2_ustar - emitter2_vstar);

  // Set the two selected colors.
  emitterPowers[emitter1].power = I * S * abs(ustar-emitter2_ustar)/abs(emitter2_ustar - emitter1_ustar);
  emitterPowers[emitter2].power = I * S * abs(ustar-emitter1_ustar)/abs(emitter2_ustar - emitter1_ustar);

  sprintf(msg, "I: %3.2f S: %3.2f u: %3.2f v: %3.2f p1: %4.4f p2: %4.4f", I, S, ustar, vstar, emitterPowers[emitter1].power, emitterPowers[emitter2].power);
  debugPrint(msg);

  // Add white to the end, and set the power
  emitterPowers.push_back(outputEmitter(_whiteEmitter.localAddress, I * (1 - S)));

//  // For debugging, print the actual output values.
//  Serial.println("Target Hue of " + String(H) + " between LEDs " + String(LED1) + " and " + String(LED2));
//  Serial.println("Output Values");
//  for (std::vector<float>::iterator i=LEDOutputs.begin(); i != LEDOutputs.end(); ++i) {
//    Serial.print(*i, 2);
//    Serial.print(" ");
//  }
//  Serial.println("");

  return emitterPowers;
}
