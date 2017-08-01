#include "outputpca9685.h"
#include "luminous-particle.h"

#include "emitter.h"
#include "debug.h"

#include <vector>

#define PCA9685_PWM_FULL 4096

// TODO: Move creation of the PCA9685 class here, and handle cases where
// multiple HSILights are on the same PCA9685
OutputPCA9685::OutputPCA9685(TwoWire& i2cbus, uint8_t i2caddress) :
  _i2cbus(i2cbus),
  _i2caddress(i2caddress) {}

bool OutputPCA9685::initImplementation() {
  debugPrint(DEBUG_TRACE, "OutputPCA9685::initImplementation");
  debugPrint(DEBUG_INSANE, "  resetDevices");
  _pwm.resetDevices();
  debugPrint(DEBUG_INSANE, "  init");;
  _pwm.init(B000000, PCA9685_MODE_OUTPUT_ONACK | PCA9685_MODE_OUTPUT_TPOLE);
  debugPrint(DEBUG_INSANE, "  setPWMFrequency");
  _pwm.setPWMFrequency(400);
  return true;
}

bool OutputPCA9685::sleepImplementation() const {
  return true;
}

bool OutputPCA9685::allOffImplementation() const {
  return true;
}

bool OutputPCA9685::setEmitterPowers(const std::vector<outputEmitter>& emitterPowers) {
  // Note that this does not support non-contiguous updates;
  // assumes that the start address is the lowest localAddress
  // in emitterPowers

  debugPrint(DEBUG_INSANE, "OutputPCA9685::setEmitterPowers start");;

  debugPrintf(DEBUG_INSANE, "  emitterPowers.size() == %u", emitterPowers.size());

  uint16_t pwms[emitterPowers.size()];
  uint16_t minAddress = UINT16_MAX;
  uint16_t powerTo9685;

  for (uint16_t i = 0; i < emitterPowers.size(); i++) {
    if (emitterPowers[i].outputLocalAddress < minAddress) {
      minAddress = emitterPowers[i].outputLocalAddress;
    }
  }

  debugPrintf(DEBUG_INSANE, "  minAddress == %u", minAddress);

  for (uint16_t i = 0; i < emitterPowers.size(); i++) {
    if ((globalBrightness == 1.0f) && (emitterPowers[i].power == 1.0f)) {
      powerTo9685 = PCA9685_PWM_FULL;
    } else {
      powerTo9685 = (uint16_t)(4096.0f * globalBrightness * emitterPowers[i].power) & 0x0FFF;
    }
    debugPrintf(DEBUG_INSANE,
                "  setting pwms[%u] to %u for i %u, la %u",
                emitterPowers[i].outputLocalAddress - minAddress,
                powerTo9685,
                i,
                emitterPowers[i].outputLocalAddress);
    pwms[emitterPowers[i].outputLocalAddress - minAddress] = powerTo9685;
  }
  _pwm.setChannelsPWM(minAddress, emitterPowers.size(), pwms);
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
