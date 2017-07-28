#ifndef DEBUG_H
#define DEBUG_H

#include "Particle.h"
#include <stdarg.h>

#define PRINTF_BUFFER_SIZE 256

#ifdef DEBUG

void Serial_printf(const char* fmt, ...);

#define debug_print(fmt, ...) \
        do { if (DEBUG) Serial_printf("%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#endif

static bool debugOutputMode = true;

void debugPrint(const char* text);

void setDebugOutput(bool newMode);
bool getDebugOutput();

#endif
