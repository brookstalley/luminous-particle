//*********************************************************
//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

#include "Emitter.h"

Emitter::Emitter(const char *name, float u, float v, float maxvalue) :
  _u(u),
  _v(v),
  _name(name),
  _maxvalue(maxvalue)
}

Emitter::Emitter(void) {
}

float Emitter::getU(void) {
  return _u;
}

float Emitter::getV(void) {
  return _v;
}

float Emitter::getMax(void) {
  return _maxvalue;
}

char * Emitter::getName(void) {
  return _name;
}
