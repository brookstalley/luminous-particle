#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include <vector>

#include "emitter.h"
#include "debug.h"

extern float globalBrightness;

class OutputInterface {

  protected:
    virtual bool initImplementation();
    virtual bool sleepImplementation();
    virtual bool allOffImplementation();

  public:
    virtual bool setEmitterPowers(std::vector<outputEmitter> emitters) const = 0;

    bool init() {initImplementation(); return true;};
    bool sleep();
    bool allOff();

};

#endif
