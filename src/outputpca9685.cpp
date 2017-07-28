#include "outputpca9685.h"

#define PCA9685_PWM_FULL 4096

OutputPCA9685::OutputPCA9685(TwoWire &i2cbus, uint8_t i2caddress) :
  _i2cbus(i2cbus),
  _i2caddress(i2caddress) {
}

bool OutputPCA9685::initImplementation() {
  debugPrint("  resetDevices");
  _pwm.resetDevices();
  debugPrint("  init");
  _pwm.init(B000000, PCA9685_MODE_OUTPUT_ONACK | PCA9685_MODE_OUTPUT_TPOLE);
  debugPrint("  setPWMFrequency");
  _pwm.setPWMFrequency(400);
  return true;
}

bool OutputPCA9685::sleepImplementation() const {
  return true;
}

bool OutputPCA9685::allOffImplementation() const {
  return true;
}

bool OutputPCA9685::setEmitterPowers(const std::vector<outputEmitter>& emitterPowers) const {
  // Note that this does not support non-contiguous updates;
  // assumes that the start address is the lowest localAddress
  // in emitterPowers

  debugPrint("OutputPCA9685::setEmitterPowers start");
  char msg[100];
  sprintf(msg,"  emitterPowers.size() == %u", emitterPowers.size());
  debugPrint(msg);

  uint16_t pwms[emitterPowers.size()];
  uint16_t minAddress = UINT16_MAX;
  uint16_t powerTo9685;

  for (uint16_t i = 0; i < emitterPowers.size(); i++) {
    if (emitterPowers[i].localAddress < minAddress) {
      minAddress = emitterPowers[i].localAddress;
    }
  }

  sprintf(msg,"  minAddress == %u", minAddress);
  debugPrint(msg);

  for (uint16_t i = 0; i < emitterPowers.size(); i++) {
    if ((globalBrightness == 1.0f) && (emitterPowers[i].power == 1.0f)) {
      powerTo9685 = PCA9685_PWM_FULL;
    } else {
      powerTo9685 = (uint16_t)(4096.0f * globalBrightness * emitterPowers[i].power) & 0x0FFF;
    }
    sprintf(msg,"  setting pwms[%u] to %u for i %u, la %u", emitterPowers[i].localAddress - minAddress, powerTo9685, i, emitterPowers[i].localAddress);
    debugPrint(msg);
    pwms[emitterPowers[i].localAddress - minAddress] = powerTo9685;
    // _pwm.setChannelsPWM(pwms, minAddress);
  }
  return true;
}

bool OutputPCA9685::init() {
  return initImplementation();
}

bool OutputPCA9685::sleep() const {
  return sleepImplementation();
}

bool OutputPCA9685::allOff() const {
  return allOffImplementation();
}
