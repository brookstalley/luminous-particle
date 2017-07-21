//*********************************************************
//
// CIE Light Library
// Copyright Brooks Talley 2017
//
// This is a derivative work based on Brian
// Neltner's TeensyLED Controller Library:
// https://github.com/saikoLED/TeensyLED

#ifndef EMITTER_H
#define EMITTER_H

class Emitter {
  private:
    float _u, _v, _maxvalue;
    char (&_name)[10];
  public:
    Emitter(char (&name)[10], float u, float v, float maxvalue);
    Emitter(void);
    float getU(void);
    float getV(void);
    float getMax(void);
    char* getName(void);
};
#endif
