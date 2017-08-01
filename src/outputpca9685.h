#ifndef OUTPUTPCA9685_H
#define OUTPUTPCA9685_H

#include "outputinterface.h"
#include "PCA9685-Particle/PCA9685.h"
#include "emitter.h"

#include "Particle.h"

#include <vector>

class OutputPCA9685: public OutputInterface {
private:

  TwoWire& _i2cbus;
  PCA9685  _pwm;
  uint8_t  _i2caddress;

  bool initImplementation();
  bool sleepImplementation() const;
  bool allOffImplementation() const;

public:

  OutputPCA9685(TwoWire & i2cbus, uint8_t i2caddress);

  bool setEmitterPowers(const std::vector < outputEmitter >& emitters,
                        float scaleFactor);

  bool init();
  bool sleep() const;
  bool allOff() const;
};

#endif /* ifndef OUTPUTPCA9685_H */
