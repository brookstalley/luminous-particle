#ifndef TEMPERATUREADS1115_H
#define TEMPERATUREADS1115_H

#include "temperatureinterface.h"
#include "Adafruit_ADS1X15/Adafruit_ADS1015.h"

#include "Particle.h"

class TemperatureAds1115: public TemperatureInterface {
private:

  TwoWire& _i2cbus;
  Adafruit_ADS1015 _adc;
  uint8_t _i2caddress;

  bool initImplementation();

public:

  TemperatureAds1115(TwoWire & i2cbus, uint8_t i2caddress);
  float getTemperature(uint16_t temperatureLocalAddress);
  bool  init();
};

#endif /* ifndef TEMPERATUREADS1115_H */
