#include "temperatureads1115.h"

#include "Particle.h"

TemperatureAds1115::TemperatureAds1115(TwoWire& i2cbus, uint8_t i2caddress) :
  _i2cbus(i2cbus),
  _i2caddress(i2caddress) {}

bool TemperatureAds1115::initImplementation() {
 {
    debugPrint(DEBUG_TRACE, "TemperatureAds1115::initImplementation");
/* From PCA9685
    _pwm.resetDevices();
    debugPrint(DEBUG_INSANE, "  init");;
    _pwm.init(B000000, PCA9685_MODE_OUTPUT_ONACK | PCA9685_MODE_OUTPUT_TPOLE);
    debugPrint(DEBUG_INSANE, "  setPWMFrequency");
    _pwm.setPWMFrequency(400);
    */
    return true;
  }
}

float TemperatureAds1115::getTemperature(uint16_t localAddress) {
  return 123.4f;
}

bool TemperatureAds1115::init() {
  return initImplementation();
}
