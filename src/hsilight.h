#ifndef HSILIGHT_H
#define HSILIGHT_H

#include "compositemodule.h"
#include "outputinterface.h"
#include "temperatureinterface.h"

#include "E131/E131.h"

#include "Particle.h"

#define TEMPERATURE_VALID_MILLIS 1000

class HSILight {
private:

  const char *_name;
  const CompositeModule& _compositeModule;
  std::shared_ptr < OutputInterface > _outputInterface;
  const uint16_t _outputLocalAddress;
  const E131   & _e131;
  const uint16_t _e131LocalAddress;
  std::shared_ptr < TemperatureInterface > _temperatureInterface;
  const uint16_t _temperatureLocalAddress;

  std::vector < outputEmitter > _emitterPowers;
  float _localBrightness;
  float _temperature;

  unsigned long _tempertureUpdatedMillis;

  void setEmitters();

public:

  HSILight(const char *name,
           const CompositeModule &compositemodule,
           std::shared_ptr < OutputInterface > outputInterface,
           const uint16_t outputLocalAddress,
           const E131 e131,
           const uint16_t e131LocalAddress, // TODO: Add multiple universe
                                            // support
           std::shared_ptr < TemperatureInterface > temperatureInterface,
           const uint16_t temperatureLocalAddress);

  HSILight(const char *name,
           const CompositeModule &compositemodule,
           std::shared_ptr < OutputInterface > outputInterface,
           const uint16_t outputLocalAddress,
           const E131 e131,
           const uint16_t e131LocalAddress); // TODO: Add multiple universe
                                             // support

  void        begin();
  void        setColor(const HSIColor& color);
  void        setColorFromE131();
  void        getColor(const HSIColor *) const;
  void        setTemperatureInterface(std::shared_ptr < TemperatureInterface > temperatureInterface);

  float       getTemperature();
  float       updateTemperature();
  const char* getName(void) const;
  void        setSingleEmitterOn(unsigned int index);
};
#endif /* ifndef HSILIGHT_H */
