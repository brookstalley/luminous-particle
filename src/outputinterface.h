#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include "emitter.h"

#include <vector>

extern float globalBrightness;

class OutputInterface {
public:

  virtual bool setEmitterPowers(const std::vector < outputEmitter >& emitters) = 0;

  virtual bool init()         = 0;
  virtual bool sleep() const  = 0;
  virtual bool allOff() const = 0;
};

#endif /* ifndef OUTPUTINTERFACE_H */
