#include "debug.h"

void debugPrint(uint16_t level, const char* text) {
  if (level >= debugOutputMode) {
    Serial.println(text);
  }
}

void debugPrintf(uint16_t level, const char* fmt, ...) {
  if (level >=debugOutputMode) {
    char buff[PRINTF_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buff, PRINTF_BUFFER_SIZE, fmt, args);
    va_end(args);
    Serial.println(buff);
  }
}

uint16_t getDebugOutput() {
  return debugOutputMode;
}

void setDebugOutput(uint16_t newMode) {
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
