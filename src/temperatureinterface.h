#ifndef TEMPERATUREINTERFACE_H
#define TEMPERATUREINTERFACE_H

#include "Particle.h"

class TemperatureInterface {
public:

  virtual float getTemperature(uint16_t localAddress) = 0;

  virtual bool  begin() = 0;
};


#endif /* ifndef TEMPERATUREINTERFACE_H */
