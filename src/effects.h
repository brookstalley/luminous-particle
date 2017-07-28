#ifndef EFFECTS_H
#define EFFECTS_H

#include <math.h>
#include <algorithm>

#include "Particle.h"
#include "hsilight.h"
#include "hsicolor.h"
#include "debug.h"

bool effectTest(std::vector<std::shared_ptr<HSILight>> lights, bool lightsMustUpdate);

bool effectRotate(std::vector<std::shared_ptr<HSILight>> lights, bool lightsMustUpdate);

#endif
