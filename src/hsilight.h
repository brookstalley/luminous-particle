#ifndef HSILIGHT_H
#define HSILIGHT_H

#include "compositemodule.h"
#include "outputinterface.h"
#include "temperatureinterface.h"

#include "Particle.h"

#define TEMPERATURE_VALID_MILLIS 1000

class HSILight {
private:

  const char *_name;
  const CompositeModule& _compositeModule;
  OutputInterface& _outputInterface;
  const uint16_t   _outputLocalAddress;
  TemperatureInterface *_temperatureInterface;
  const uint16_t _temperatureLocalAddress;

  std::vector < outputEmitter > _emitterPowers;
  float _localBrightness;
  float _temperature;

  unsigned long _tempertureUpdatedMillis;

  void setEmitters();

public:

  HSILight(const char *name,
           const CompositeModule &compositemodule,
           OutputInterface & outputInterface,
           const uint16_t outputLocalAddress,
           TemperatureInterface & temperatureInterface,
           const uint16_t temperatureLocalAddress);
  HSILight(const char *name,
           const CompositeModule &compositemodule,
           OutputInterface & outputInterface,
           const uint16_t outputLocalAddress,
           );
  void        begin();
  void        setColor(const HSIColor& color);
  void        getColor(const HSIColor *) const;
  float       getTemperature() const;
  const char* getName(void) const;
  void        setSingleEmitterOn(unsigned int index);
};
#endif /* ifndef HSILIGHT_H */
