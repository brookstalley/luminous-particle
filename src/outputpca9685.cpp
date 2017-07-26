#include "outputpca9685.h"


bool OutputPCA9685::initImplmentation() {
  _pwm = PCA9685(Wire);
}

bool OutputPCA9685::sleepImplementation() {

}

bool OutputPCA9685::allOffImplementation() {

}

bool OutputPCA9685::sendCommands(std::vector<outputEmitter> emitters) {

}
