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

#ifdef DEBUG

void Serial_printf(const char* fmt, ...) {
   char buff[PRINTF_BUFFER_SIZE];
   va_list args;
   va_start(args, fmt);
   vsnprintf(buff, PRINTF_BUFFER_SIZE, fmt, args);
   va_end(args);
   Serial.println(buff);
}

#endif
