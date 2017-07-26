#ifndef OUTPUTPCA9685_H
#define OUTPUTPCA9685_H

#include "PCA9685-Particle/PCA9685-Particle.h"

class OutputPCA9685 : OutputInterface {
  private:
    TwoWire &_i2cbus;
    PCA9685 _pwm;
    uint8_t _i2caddress;

  protected:
    bool initImplmentation();
    bool sleepImplementation();
    bool allOffImplementation();

  public:
    void OutputPCA9685(TwoWire &i2cbus, uint8_t i2caddress);

    bool setEmitterPowers(std::vector<outputEmitter> emitterOutputs) = 0;

    bool init(TwoWire& i2cbus);
    bool sleep();
    bool allOff();

};

#endif
