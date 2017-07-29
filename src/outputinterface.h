#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include <vector>

#include "emitter.h"
#include "debug.h"

extern float globalBrightness;

class OutputInterface {

  public:
    virtual bool setEmitterPowers(const std::vector<outputEmitter>& emitters) = 0;

    virtual bool init() = 0;
    virtual bool sleep() const = 0;
    virtual bool allOff() const = 0;

};

#endif
