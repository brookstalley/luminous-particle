#ifndef MODE_H
#define MODE_H

#include <vector>
#include <memory>

#include "HSILight.h"

class Mode {
private:

  const char *_name;

public:

  Mode(const char *name);
  virtual ~Mode() {};

  virtual bool start(std::vector < std::shared_ptr < HSILight >> lights);
  virtual bool run(std::vector < std::shared_ptr < HSILight >> lights,
                   bool lightsMustUpdate);
  virtual bool end(std::vector < std::shared_ptr < HSILight >> lights);

  const char * getName();
};

#endif /* ifndef MODE_H */
