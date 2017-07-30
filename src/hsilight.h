#ifndef HSILIGHT_H
#define HSILIGHT_H

#include "hsicolor.h"
#include "compositemodule.h"
#include "outputinterface.h"
#include "debug.h"

class HSILight {
private:

  const CompositeModule& _compositeModule;
  OutputInterface& _interface;
  const uint16_t   _localAddress;
  const char *_name;

  std::vector < outputEmitter > _emitterPowers;

  void setEmitters();

public:

  HSILight(const char *name,
           const CompositeModule &compositemodule,
           OutputInterface & interface,
           const uint16_t localAddress);
  void        begin();
  void        setColor(const HSIColor& color);
  void        getColor(const HSIColor *) const;
  const char* getName(void) const;
  void        setSingleEmitterOn(unsigned int index);
};
#endif /* ifndef HSILIGHT_H */
