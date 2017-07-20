#include "hsilamp.h"

HSILamp::HSILamp(std::shared_ptr<Colorspace> colorspace) :
  _colorspace(colorspace),
  _maxvalues = colorspace->getMaxValues()
{

}

void HSILamp::setColor(HSIColor &color) {
  std::vector<float> emitters = _colorspace->Hue2EmitterPower(color);
  for (int i=0; i<emitters.size(); i++) {
    emitters[i] = _maxvalues[i] * emitters[i];
  }
  setEmitters(emitters, _pins);
}

void HSILamp::setEmitters(std::vector<float> &emitters) {
  for (int i=0; i<emitters.size(); i++) {
    analogWrite(pins[i], 0xFFFF * emitters[i] * globalBrightness);
  }

}
