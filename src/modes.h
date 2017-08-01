#ifndef MODES_H
#define MODES_H

#include "luminous-particle.h"

#include "mode.h"
#include "hsilight.h"
#include "hsicolor.h"
#include "debug.h"

#include "Particle.h"

#include <vector>
#include <memory>

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

void        nextMode();
bool        setModeByNumber(uint16_t modeNumber);
bool        setModeByName(String modeName);
void        runCurrentMode();
const char* getCurrentModeName();

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
