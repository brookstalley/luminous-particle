#include "debug.h"

uint16_t debugOutputMode = DEBUG_ERROR;

void debugPrint(uint16_t level, const char *buffer) {
  static const char *eventName = "debug";

  // If it's of higher priority (lower number) than our current mode, ....
  if (level <= debugOutputMode) {
    // Always print to serial
    Serial.println(buffer);

    // If we're connected, and if it's a serious message, send to Particle
    // Don't sent TRACE or INSANE messages because Particle is rate-limited
    // to 1 message per second
    if (Particle.connected()) {
      if (level <= DEBUG_WARN) {
        Particle.publish(eventName, buffer, 60, PRIVATE);
      }
    }
  }
}

void debugPrintf(uint16_t level, const char* fmt, ...) {
  if (level >=debugOutputMode) {
    char buff[PRINTF_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buff, PRINTF_BUFFER_SIZE, fmt, args);
    va_end(args);
    debugPrint(level, buff);
  }
}

uint16_t getDebugOutput() {
  return debugOutputMode;
}

void setDebugOutput(uint16_t newMode) {
  debugOutputMode = newMode;
}

#ifdef DEBUG_BUILD

void Serial_printf(const char* fmt, ...) {
   char buff[PRINTF_BUFFER_SIZE];
   va_list args;
   va_start(args, fmt);
   vsnprintf(buff, PRINTF_BUFFER_SIZE, fmt, args);
   va_end(args);
   Serial.println(buff);
}

#endif
