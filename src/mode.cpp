#include "mode.h"

Mode::Mode(const char *name) :
  _name(name) {}

bool Mode::start(std::vector<std::shared_ptr<HSILight> >lights) {
  return true;
}

bool Mode::run(std::vector<std::shared_ptr<HSILight> >lights, bool lightsMustUpdate) {
  return true;
}

bool Mode::end(std::vector<std::shared_ptr<HSILight> >lights) {
  return true;
}

const char * Mode::getName() {
  return _name;
}
