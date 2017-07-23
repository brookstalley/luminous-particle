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
  _maxvalue(maxvalue),
  _name(name)
{
}

float Emitter::getU(void) const {
  return _u;
}

float Emitter::getV(void) const {
  return _v;
}

float Emitter::getMax(void) const {
  return _maxvalue;
}

const char * Emitter::getName(void) const {
  return _name;
}
