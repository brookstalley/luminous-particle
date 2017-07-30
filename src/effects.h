#ifndef EFFECTS_H
#define EFFECTS_H

#include <math.h>
#include <algorithm>

#include "Particle.h"
#include "hsilight.h"
#include "hsicolor.h"
#include "debug.h"

typedef bool (*luminousFunctionPointer) (std::vector < std::shared_ptr < HSILight >>,
                                         bool);

typedef struct {
  const char              name[10];
  luminousFunctionPointer startPointer;
  luminousFunctionPointer runPointer;
  luminousFunctionPointer endPointer;
} luminousMode;

extern const int modeCount;

extern luminousMode modes[];

extern uint16_t currentMode;

// TODO: This is a mess. Effects should probably be classes with an common
// interface.

bool startOff(std::vector < std::shared_ptr < HSILight >> lights,
              bool lightsMustUpdate);

bool runOff(std::vector < std::shared_ptr < HSILight >> lights,
            bool lightsMustUpdate);

bool endOff(std::vector < std::shared_ptr < HSILight >> lights,
            bool lightsMustUpdate);

bool startTest(std::vector < std::shared_ptr < HSILight >> lights,
               bool lightsMustUpdate);

bool runTest(std::vector < std::shared_ptr < HSILight >> lights,
             bool lightsMustUpdate);

bool endTest(std::vector < std::shared_ptr < HSILight >> lights,
             bool lightsMustUpdate);

bool startRotate(std::vector < std::shared_ptr < HSILight >> lights,
                 bool lightsMustUpdate);

bool runRotate(std::vector < std::shared_ptr < HSILight >> lights,
               bool lightsMustUpdate);

bool endRotate(std::vector < std::shared_ptr < HSILight >> lights,
               bool lightsMustUpdate);

bool startE131(std::vector < std::shared_ptr < HSILight >> lights,
               bool lightsMustUpdate);

bool runE131(std::vector < std::shared_ptr < HSILight >> lights,
             bool lightsMustUpdate);

bool endE131(std::vector < std::shared_ptr < HSILight >> lights,
             bool lightsMustUpdate);

#endif /* ifndef EFFECTS_H */
