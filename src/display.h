// *********************************************************

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
// **********************************************************

#ifndef LUMINOUS_DISPLAY_H
#define LUMINOUS_DISPLAY_H

#include "Adafruit-GFX-library/Adafruit_GFX.h"

// Color definitions
#define DISPLAY_BLACK           0x0000
#define DISPLAY_BLUE            0x001F
#define DISPLAY_RED             0xF800
#define DISPLAY_GREEN           0x07E0
#define DISPLAY_CYAN            0x07FF
#define DISPLAY_MAGENTA         0xF81F
#define DISPLAY_YELLOW          0xFFE0
#define DISPLAY_WHITE           0xFFFF


class Display {
private:

  Adafruit_GFX& _screen;
  uint8_t _width;
  uint8_t _height;
  uint8_t _lineHeight;
  uint8_t _charWidth;
  uint8_t _charsPerLine;
  uint8_t _maxLines;
  uint8_t _nextLine;

public:

  Display(Adafruit_GFX,
          width,
          height);

  void startPage(bool clearDisplay);

  void println(
    uint16_t    fontColor,
    uint16_t    backColor,
    const char *lineData,
    ...);

  void clear();
  void setTop();
}

#endif // ifndef LUMINOUS_DISPLAY_H
