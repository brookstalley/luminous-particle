//*********************************************************
//
// Luminous
// Copyright 2017 Brooks Talley
//
// Portions derived from TeensyLED, copyright 2015 Brian Neltner
//
// Luminous is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Luminous is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Luminous.  If not, see <http://www.gnu.org/licenses/>.
//
//**********************************************************

// Do not try to change this file to debug.h, or use DEBUG_H as the
// include guard. It will make you very unhappy.
#ifndef LUMINOUS_DEBUG_H
#define LUMINOUS_DEBUG_H

#include "Particle.h"

#define PRINTF_BUFFER_SIZE 256

enum DEBUG_LEVEL {
	DEBUG_MANDATORY = 0,
	DEBUG_ERROR,
	DEBUG_WARN,
	DEBUG_TRACE,
	DEBUG_INSANE,
	END_OF_LIST
};

#ifdef DEBUG_BUILD

#include <stdarg.h>

void Serial_printf(const char* fmt, ...);

#define debug_print(fmt, ...) \
	do { if (DEBUG_LEVEL) Serial_printf("%s:%d:%s(): " fmt, __FILE__, \
		                            __LINE__, __func__, __VA_ARGS__); } while (0)

#endif


//TODO: make debugPrint and debugPrintf into macros that don't produce
//      code if DEBUG_BUILD is not defined
void debugPrint(uint16_t level, const char* text);
void debugPrintf(uint16_t level, const char* fmt, ...);

void setDebugLevel(uint16_t newMode);

uint16_t getDebugLevel();
void getDebugLevelName(uint16_t level, char *buffer, size_t buffer_length);


#endif
