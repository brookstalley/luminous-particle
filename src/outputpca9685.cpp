// *********************************************************

//
// Luminous
// Copyright 2017 Brooks Talley
//
// Portions derived from TeensyLED, copyright 2015 Brian Neltner
//
// Luminous is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Luminous is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Luminous.  If not, see <http://www.gnu.org/licenses/>.
//
// **********************************************************
#include "outputpca9685.h"
#include "luminous-particle.h"

#include "emitter.h"
#include "ldebug.h"

#include <vector>

#define PCA9685_PWM_FULL 4096

// TODO: Move creation of the PCA9685 class here, and handle cases where
// multiple HSILights are on the same PCA9685
OutputPCA9685::OutputPCA9685(TwoWire& i2cbus, uint8_t i2caddress) :
  _i2cbus(i2cbus),
  _i2caddress(i2caddress) {}

bool OutputPCA9685::initImplementation() {
  debugPrint(DEBUG_TRACE,  "OutputPCA9685::initImplementation");
  debugPrint(DEBUG_INSANE, "  resetDevices");
  _pwm.resetDevices();
  debugPrint(DEBUG_INSANE, "  init");
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

bool OutputPCA9685::setEmitterPowers(const std::vector<outputChannel>& outputChannels, float scaleFactor) {
  // Note that this does not support non-contiguous updates;
  // assumes that the start address is the lowest localAddress
  // in emitterPowers

  debugPrintf(DEBUG_INSANE,
              "OutputPCA9685::setEmitterPowers start. EmitterPowers.size() == %u, scale %f",
              outputChannels.size(), scaleFactor);

  uint16_t pwms[outputChannels.size()];
  uint16_t minAddress = UINT16_MAX;
  uint16_t powerTo9685;

  for (uint16_t i = 0; i < outputChannels.size(); i++) {
    if (outputChannels[i].outputLocalAddress < minAddress) {
      minAddress = outputChannels[i].outputLocalAddress;
    }
  }

  debugPrintf(DEBUG_INSANE, "  minAddress == %u", minAddress);

  for (uint16_t i = 0; i < outputChannels.size(); i++) {
    if ((scaleFactor == 1.0f) && (outputChannels[i].power == 1.0f)) {
      powerTo9685 = PCA9685_PWM_FULL;
    } else {
      powerTo9685 = (uint16_t)(4096.0f * scaleFactor * outputChannels[i].power) & 0x0FFF;
    }
    debugPrintf(DEBUG_INSANE,
                "  setting pwms[%u] to %u for i %u, la %u",
                outputChannels[i].outputLocalAddress - minAddress,
                powerTo9685,
                i,
                outputChannels[i].outputLocalAddress);
    pwms[outputChannels[i].outputLocalAddress - minAddress] = powerTo9685;
  }
  _pwm.setChannelsPWM(minAddress, outputChannels.size(), pwms);
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
