#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include "emitter.h"

class OutputInterface {

  protected:
    OutputInterface() {};
    virtual bool initImplmentation();
    virtual bool sleepImplementation();
    virtual bool allOffImplementation();

  public:
    virtual ~OutputInterface() {};
    virtual bool SendCommands(std::vector<outputEmitter> emitters) = 0;

    bool init();
    bool sleep();
    bool allOff();

};





#endif
