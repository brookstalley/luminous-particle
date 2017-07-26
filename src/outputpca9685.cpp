#include "outputpca9685.h"

#define PCA9685_PWM_FULL 4096

void OutputPCA9685(TwoWire &i2cbus, uint8_t i2caddress) :
  _i2cbus(i2cbus),
  _i2caddress(i2caddress) {
}

bool OutputPCA9685::initImplmentation() {
  debugPrint("  resetDevices");
  _pwm.resetDevices();
  debugPrint("  init");
  _pwm.init(B000000, PCA9685_MODE_OUTPUT_ONACK | PCA9685_MODE_OUTPUT_TPOLE);
  debugPrint("  setPWMFrequency");
  _pwm.setPWMFrequency(400);

}

bool OutputPCA9685::sleepImplementation() {

}

bool OutputPCA9685::allOffImplementation() {

}

bool OutputPCA9685::setEmitterPowers(std::vector<outputEmitter> emitterPowers) {
  // Note that this does not support non-contiguous updates;
  // assumes that the start address is the lowest localAddress
  // in emitterPowers
  uint16_t pwms[emitterPowers.size()];
  uint16_t minAddress = UINT16_MAX;
  uint16_t powerTo9685;

  for (uint16_t i = 0; i < emitterPowers.size(); i++) {
    if (emitterPowers[i].localAddress < minAddress) {
      minAddress = emitterPowers[i];
    }
  }
  for (uint16_t i = 0; i < emitterPowers.size(); i++) {
    if ((globalBrightness == 1.0f) && (emitterPowers[i].power = 1.0f)) {
      powerTo9685 = PCA9685_PWM_FULL
    } else {
      powerTo9685 = (4096.0f * globalBrightness * emitterPowers[i].power) & 0x0FFF
    }
    pwms[emitterPowers[i].localAddress - minAddress] = powerTo9685;

  }

}