#ifndef DEBUG_H
#define DEBUG_H

#include "Particle.h"
#include <stdarg.h>

#define PRINTF_BUFFER_SIZE 256

#ifdef DEBUG_BUILD

void Serial_printf(const char* fmt, ...);

#define debug_print(fmt, ...) \
        do { if (DEBUG_LEVEL) Serial_printf("%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#endif


enum DEBUG_LEVEL {
  DEBUG_MANDATORY = 0,
  DEBUG_ERROR = 1,
  DEBUG_WARN = 2,
  DEBUG_TRACE = 3,
  DEBUG_INSANE = 4
};

extern uint16_t debugOutputMode;

void debugPrint(uint16_t level, const char* text);
void debugPrintf(uint16_t level, const char* fmt, ...);

void setDebugOutput(uint16_t newMode);
uint16_t getDebugOutput();

#endif
