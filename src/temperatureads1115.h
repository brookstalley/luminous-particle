#ifndef TEMPERATUREADS1115_H
#define TEMPERATUREADS1115_H

#include "temperatureinterface.h"
#include "Adafruit_ADS1X15/Adafruit_ADS1X15.h"

#include "Particle.h"

class TemperatureAds1115 : TemperatureInterface {
private:
  TwoWire &_i2cbus;
  ADS1X15 _adc;
  uint8_t _i2caddress;

    bool initImplementation();

public:

   TemperatureAds1115(TwoWire &i2cbus, uint8_t i2caddress);
   bool getTemperature(uint16_t localAddress);
   bool init();
};

#endif
