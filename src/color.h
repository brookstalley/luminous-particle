/* */

#ifndef LUMINOUS_COLOR_H
#define LUMINOUS_COLOR_H

#include "Particle.h"

struct cieXYcolor {
  float x;
  float y;

  cieXYcolor(float inx = 0.0f,
             float iny = 0.0f) : x(inx), y(iny) {}
};

struct cieUVcolor {
  float u;
  float v;

  cieUVcolor(float inu = 0.0f,
             float inv = 0.0f) : u(inu), v(inv) {}
};

cieXYcolor cieXYfromWavelength(uint16_t);
cieUVcolor cieUVfromWavelength(uint16_t);
cieUVcolor cieUVfromXY(const cieXYcolor);

extern float const wavelengthToXY[830 - 360 + 1][2];

#endif // ifndef LUMINOUS_COLOR_H
