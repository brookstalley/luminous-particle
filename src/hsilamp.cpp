#include "hsilamp.h"

HSILamp::HSILamp() :
}

void HSILamp::setColor(HSIColor &color) {
  std::vector<float> emitters = _colorspace->Hue2EmitterPower(color);
  for (int i=0; i<emitters.size(); i++) {
    emitters[i] = _maxvalues[i] * emitters[i];
  }
  setLEDs(LEDs, _pins);
}

void HSILamp::setLEDs(std::vector<float> &emitters) {
  for (int i=0; i<emitters.size(); i++) {
    analogWrite(pins[i], 0xFFFF * emitters[i]);
  }

}

void HSILamp::addColorspace(std::shared_ptr<Colorspace> colorspace) {  
  _maxvalues = colorspace->getMaxValues();
  _colorspace = colorspace;
}
