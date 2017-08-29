/* */

struct cieXYcolor {
  cieXYcolor(float inx, float iny) {
    x = inx;
    y = iny;
  }

  float x;
  float y;
}

struct cieUVcolor {
  cieUVcolor(float inu, float inv) {
    x = inu;
    y = inv;
  }

  float u;
  float v;
}

cieXYcolor cieXYfromWavelength(uint16_t wavelength);
cieUVcolor cieUVfromWavelength(uint16_t wavelength);
cieUVcolor cieUVfromXY(cieXYcolor);

extern float const wavelengthToXY[830 - 360 + 1][2];
