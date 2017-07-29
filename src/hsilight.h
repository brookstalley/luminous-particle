#ifndef HSILIGHT_H
#define HSILIGHT_H

#include "hsicolor.h"
#include "compositemodule.h"
#include "outputinterface.h"
#include "debug.h"

class HSILight {
  private:
    const CompositeModule &_compositeModule;
    OutputInterface &_interface;
    const uint16_t _localAddress;

    std::vector<outputEmitter> _emitterPowers;

    void setEmitters();

  public:
    HSILight(const CompositeModule &compositemodule,  OutputInterface &interface, const uint16_t localAddress);
    void begin();
    void setColor(const HSIColor &color);
    void getColor(const HSIColor*) const;
    void setSingleEmitterOn(unsigned int index);

    void debugOutput() const;
};
#endif
