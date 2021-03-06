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
#ifndef HSILIGHT_H
#define HSILIGHT_H


#include "compositemodule.h"
#include "outputinterface.h"
#include "temperatureinterface.h"

#include "E131/E131.h"

#include "Particle.h"

#define HSILIGHT_MAX_DIAGNOSTIC 30
#define TEMPERATURE_VALID_MILLIS 1000

class HSILight {
private:

  const char *_name;
  const CompositeModule& _compositeModule;

  std::shared_ptr<OutputInterface>_outputInterface;
  const uint16_t _outputLocalAddress;

  const std::shared_ptr<E131>_e131;
  const uint16_t _e131LocalAddress;

  std::shared_ptr<TemperatureInterface>_temperatureInterface;
  const uint16_t _temperatureLocalAddress;

  std::vector<outputChannel>_outputChannels;

  float _localBrightness;

  float _temperature;
  unsigned long _tempertureUpdatedMillis;

  char _diagnostic[HSILIGHT_MAX_DIAGNOSTIC];

  uint32_t _lastE131PacketCount;
  HSIColor _lastColor;

  void setEmitters();
  void debugOutputChannels(uint16_t debugLevel);

public:

  HSILight(const char                          *name,
           const CompositeModule              & compositemodule,
           std::shared_ptr<OutputInterface>     outputInterface,
           const uint16_t                       outputLocalAddress,
           const std::shared_ptr<E131>          e131,
           const uint16_t                       e131LocalAddress, // TODO: Add multiple universe
                                                                  // support
           std::shared_ptr<TemperatureInterface>temperatureInterface,
           const uint16_t                       temperatureLocalAddress);

  HSILight(const char                     *name,
           const CompositeModule         & compositemodule,
           std::shared_ptr<OutputInterface>outputInterface,
           const uint16_t                  outputLocalAddress,
           const std::shared_ptr<E131>     e131,
           const uint16_t                  e131LocalAddress); // TODO: Add multiple universe
                                                              // support

  void                            begin();
  void                            setColor(const HSIColor& color);
  const HSIColor                & getColor() const;
  void                            setTemperatureInterface(std::shared_ptr<TemperatureInterface>temperatureInterface);
  std::shared_ptr<E131>           getE131() const;
  const uint16_t                  getE131LocalAddress() const;
  float                           getTemperature();
  float                           updateTemperature();
  const char                    * getName(void) const;
  const char                    * getDiagnostic(void) const;
  const float                     getLocalBrightness() const;
  void                            setSingleEmitterOn(unsigned int index);
  const std::vector<outputChannel>getOutputChannels() const;
};


#endif /* ifndef HSILIGHT_H */
