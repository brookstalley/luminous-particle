#ifndef TEMPERATUREINTERFACE_H
#define TEMPERATUREINTERFACE_H

class TemperatureInterface {
public:

  virtual bool getTemperature(uint16_t localAddress) = 0;

  virtual bool init()         = 0;
};


#endif
