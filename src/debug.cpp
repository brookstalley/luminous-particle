#include "debug.h"

void debugPrint(const char* text) {
  if (debugOutputMode)
    Serial.println(text);
}

bool getDebugOutput() {
  return debugOutputMode;
}

void setDebugOutput(bool newMode) {
  debugOutputMode = newMode;
}
