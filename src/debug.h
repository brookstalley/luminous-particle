#ifndef DEBUG_H
#define DEBUG_H

#include "luminous-particle.h"

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
  DEBUG_ERROR,
  DEBUG_WARN,
  DEBUG_TRACE,
  DEBUG_INSANE,
  END_OF_LIST
};

//TODO: make debugPrint and debugPrintf into macros that don't produce
//      code if DEBUG_BUILD is not defined
void debugPrint(uint16_t level, const char* text);
void debugPrintf(uint16_t level, const char* fmt, ...);

void setDebugLevel(uint16_t newMode);

uint16_t getDebugLevel();
void getDebugLevelName(uint16_t level, char *buffer, size_t buffer_length);


#endif
