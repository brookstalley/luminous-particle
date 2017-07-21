//*********************************************************
//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

struct outputEmitter {
  Emitter &emitter;
  // within a light of this type, what
  byte pwmOffset;
  float power;
}

class CompositeLight {
  private:
    std::vector<outputEmitter> _colorEmitters;
    outputEmitter _white;
    std::vector<float> _slope;
    std::vector<float> _angle;
  public:
    CompositeLight (Emitter &white, byte pwmOffset);
    void addEmitter(Emitter &emitter, byte pwmOffset);
    float getAngle(int emitternum);
    float getSlope(int emitternum);
    std::vector<outputEmitter> Hue2EmitterPower(HSIColor &HSI);
};
