#ifndef MODES_H
#define MODES_H

#include <math.h>
#include <algorithm>

#include "mode.h"
#include "Particle.h"
#include "hsilight.h"
#include "hsicolor.h"
#include "debug.h"

extern const int modeCount;

extern Mode *modes[];

extern uint16_t currentMode;

class ModeOff: public Mode {
public:

  using Mode::Mode;
  bool start(std::vector < std::shared_ptr < HSILight >>);
};

class ModeTest: public Mode {
public:

  using Mode::Mode;
  bool run(std::vector < std::shared_ptr < HSILight >>,
           bool);
};

class ModeRotate: public Mode {
public:

  using Mode::Mode;
  bool run(std::vector < std::shared_ptr < HSILight >>,
           bool);
};

void nextMode();

/*
   class ModeE131: Mode {
   public:

   bool start(std::vector < std::shared_ptr < HSILight >>);
   bool run(std::vector < std::shared_ptr < HSILight >>,
           bool);
   bool end(std::vector < std::shared_ptr < HSILight >>)
   }
 */
#endif /* ifndef MODES_H */
