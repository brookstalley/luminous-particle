#ifndef DEBUG_H
#define DEBUG_H

#include "Particle.h"

static bool debugOutputMode = true;

void debugPrint(const char* text);

void setDebugOutput(bool newMode);
bool getDebugOutput();

#endif
