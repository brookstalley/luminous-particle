#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include "emitter.h"

class OutputInterface {

  protected:
    virtual bool initImplmentation();
    virtual bool sleepImplementation();
    virtual bool allOffImplementation();

  public:
    virtual bool setEmitterPowers(std::vector<outputEmitter> emitters) const = 0;

    bool init();
    bool sleep();
    bool allOff();

};





#endif
