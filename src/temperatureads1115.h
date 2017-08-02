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

  // Note that this class assumes all thermistors on the same ADC have the same
  // characteristics
  uint16_t _seriesResistor;
  uint16_t _adcMax;
  uint16_t _thermistorNominal;
  uint16_t _temperatureNominal;
  uint16_t _bCoef;

  bool beginImplementation();

public:

  TemperatureAds1115(TwoWire & i2cbus,
                     uint8_t i2caddress,
                     uint16_t seriesResistor,
                     uint16_t thermistorNominal,
                     uint16_t temperatureNominal,
                     uint16_t bCoef);
  float getTemperature(uint16_t temperatureLocalAddress);
  bool  begin();
};

#endif /* ifndef TEMPERATUREADS1115_H */
