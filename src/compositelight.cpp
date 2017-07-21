//*********************************************************
//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

// CompositeLight models a light that contains multiple emitters.
// Constructor takes a single emitter to use as white. This will be used
// to desaturate based on HSI
//
// After constructing, add color emitters using addEmitter.

#include "compositelight.h"

CompositeLight ::CompositeLight (Emitter &white, byte pwmOffset) :
  _white({white, pwmOffset}) {
}

void CompositeLight ::addEmitter(Emitter &emitter, byte pwmOffset) {
  // To figure out where to put it in the colorspace, calculate the angle from the white point.
  float uEmitter = emitter.getU();
  float vEmitter = emitter.getV();
  float uWHITE = _white.emitter.getU();
  float vWHITE = _white.emitter.getV();

  float angle = fmod((180/M_PI) * atan2((vEmitter - vWHITE),(uEmitter - uWHITE)) + 360, 360);

  // If it is the first LED, simply place it in the array.
  if (_colorEmitters.empty()) {
    _colorEmitters.push_back(outputEmitter());
    _colorEmitters[0].emitter = emitter;
    _colorEmitters[0].pwmOffset = pwmOffset;
    _colorEmitters[0].power = 0;

    _angle.push_back(angle);
    // With only one LED, slope is undefined.
    _slope.push_back(0);
  }
  // Otherwise, place the LED at the appropriate point in the array, and also recalculate slopes.
  else {
    int insertlocation;
    // Iterate through until finding the first location where the angle fits.
    for (insertlocation = 0; (_angle[insertlocation] < angle) && (insertlocation < _angle.size()); insertlocation++);

    // Insert the new emitter and point it to the base emitter and set the pwm offset
    _colorEmitters.insert(_colorEmitters.begin() + insertlocation, outputEmitter());
    _colorEmitters[_colorEmitters.begin() + insertlocation].emitter = emitter;
    _colorEmitters[_colorEmitters.begin() + insertlocation].pwmOffset = pwmOffset;

    _angle.insert(_angle.begin() + insertlocation, angle);

    // Add an empty slope since we need to recalculate them all once they're ordered.
    _slope.push_back(0);

    // And then recalculate all slopes. Last slope is a special case.
    for (int i=0; i<(_emitters.size()-1); i++) {
      _slope[i] = (_colorEmitters[i+1].emitter.getV() - _colorEmitters[i].emitter.getV()) / (_colorEmitters[i+1].emitter.getU() - _colorEmitters[i].emitter.getU());
    }
    _slope[_colorEmitters.size()-1] = (_colorEmitters[0].emitter.getV() - _colorEmitters[_emitters.size()-1].emitter.getV()) / (_colorEmitters[0].emitter.getU() - _colorEmitters[_emitters.size()-1].emitter.getU());
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

float CompositeLight ::getAngle(int num) {
  return _angle[num];
}

std::vector<float> CompositeLight ::getMaxValues(void) {
  std::vector<float> maxvals;
  for (int i=0; i<_emitters.size(); i++) {
    maxvals.push_back(_emitters[i].getMax());
  }
  maxvals.push_back(_white.getMax());
  return maxvals;
}

std::vector<outputEmitter> CompositeLight ::Hue2EmitterPower(HSIColor &HSI) {
  float H = fmod(HSI.getHue()+360,360);
  float S = HSI.getSaturation();
  float I = HSI.getIntensity();

  float tanH = tan(M_PI*fmod(H,360)/(float)180); // Get the tangent since we will use it often.

  // Copy our emitter setup, which wills start with power being zero
  std::vector<outputEmitter> emitterPowers[] = _colorEmitters;

  int emitter1, emitter2;

  // Check the range to determine which intersection to do.
  // For angle less than the smallest CIE hue or larger than the largest, special case.

  if ((H < _angle[0]) || (H >= _angle[emitterPowers.size()-1])) {
    // Then we're mixing the lowest angle LED with the highest angle LED.
    emitter1 = emitterPowers.size() - 1;
    emitter2 = 0;
  }

  else {
    // Iterate through the angles until we find an LED with hue smaller than the angle.
    int i;
    for (i=1; (H > _angle[i]) && (i<(emitterPowers.size()-1)); i++) {
      if (H > _angle[i]) {
        emitter1 = i-1;
        emitter2 = i;
      }
    }
    emitter1 = i-1;
    emitter2 = i;
  }

  // Get the ustar and vstar values for the target LEDs.
  float emitter1_ustar = emitterPowers[emitter1].emitter.getU() - _white.getU();
  float emitter1_vstar = emitterPowers[emitter1].emitter.getV() - _white.getV();
  float emitter2_ustar = emitterPowers[emitter2].emitter.getU() - _white.getU();
  float emitter2_vstar = emitterPowers[emitter2].emitter.getV() - _white.getV();

  // Get the slope between LED1 and LED2.
  float slope = _slope[emitter1];

  float ustar = (emitter2_vstar - slope*emitter2_ustar)/(tanH - slope);
  float vstar = tanH/(slope - tanH) * (slope * emitter2_ustar - emitter2_vstar);

  // Set the two selected colors.
  emitterPowers[emitter1].power = I * S * abs(ustar-emitter2_ustar)/abs(emitter2_ustar - emitter1_ustar);
  emitterPowers[emitter2].power = I * S * abs(ustar-emitter1_ustar)/abs(emitter2_ustar - emitter1_ustar);

  // Add white to the end, and set the power
  emitterPowers.push_back(_white);
  emitterPowers[_emitters.size()].power = I * (1 - S);

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
