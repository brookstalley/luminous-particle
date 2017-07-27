#ifndef OUTPUTPCA9685_H
#define OUTPUTPCA9685_H

#include "Particle.h"
#include <algorithm>
#include <vector>

#include "PCA9685-Particle/PCA9685.h"

class OutputPCA9685 : public OutputInterface {
  private:
    TwoWire &_i2cbus;
    PCA9685 _pwm;
    uint8_t _i2caddress;

  protected:
    bool initImplmentation();
    bool sleepImplementation();
    bool allOffImplementation();

  public:
    OutputPCA9685(TwoWire &i2cbus, uint8_t i2caddress);

    bool setEmitterPowers(std::vector<outputEmitter> emitters) const;

    bool init();
    bool sleep();
    bool allOff();

};

#endif
