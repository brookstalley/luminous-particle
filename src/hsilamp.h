#include "compositelight.h"

class HSILamp {
  private:
    CompositeLight &_compositeLight;
    byte _i2cAddress;
    byte _pwmAddress;
    std::vector<outputEmitter> _emitterPowers;
  public:
    HSILamp(CompositeLight &compositelight, byte i2caddress, byte pwmAddress)
    void setColor(HSIColor &color);
    void getColor(HSIColor*);
    void setEmitters(std::vector<float> &emitters);
};
