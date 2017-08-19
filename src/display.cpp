// *********************************************************

//
// TeensyLED Controller Library
// Copyright Brian Neltner 2015
// Version 0.1 - April 13, 2015
//
// This file is part of TeensyLED Controller.
//
// TeensyLED Controller is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// TeensyLED Controller is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Luminous.  If not, see <http://www.gnu.org/licenses/>.
//
// **********************************************************

#include "display.h"
#include "ldebug.h"

#include "Adafruit-GFX-library/Adafruit_GFX.h"
#include <math.h>

Display::Display(Adafruit_GFX& screen, uint8_t width, uint8_t height) :
  _screen(screen),
  _width(width),
  _height(height) {
  _lineHeight   = 9;
  _charWidth    = 6;
  _charsPerLine = floor(_width / _charWidth);

  // Reserve one line at the top for status display
  _maxLines = (_height / _lineHeight) - 1;
  _nextLine = 1;

  char **_lineDataPrevious = 0;
  _lineDataPrevious = new char *[_maxLines + 1];

  for (uint8_t i = 0; i <= _maxLines; i++) {
    _lineDataPrevious[i] = new char[_charsPerLine + 1]();
  }
}

void Display::begin() {
  // _screen.begin();
  clear();
}

void Display::clear() {
  // Only use clear if you need the screen truly blank; it is slow
  _screen.fillScreen(DISPLAY_BLACK);
}

void Display::setTop() {
  // Use setTop for text screens where we can cache the previous content
  // and be smart about what we erase
  _nextLine = 1;
}

void Display::println(uint16_t fontColor, uint16_t backColor, const char *lineData, ...) {
  va_list arguments;
  char    thisLine[_charsPerLine + 1];

  if (_nextLine >= _maxLines) {
    debugPrintf(DEBUG_ERROR, "Display::println: exceeded screen size");
    return;
  }
  va_start(arguments, lineData);
  vsnprintf(thisLine, sizeof(thisLine), lineData, arguments);
  va_end(arguments);

  _screen.setTextColor(fontColor, backColor);

  // We always have 12 characters of label
  // So, clear the space from char 13 until the end (currently hardcoded at
  // 128)
  if (strcmp(thisLine, _lineDataPrevious[_nextLine]) != 0) {
    // Print our new text
    _screen.setCursor(0, _nextLine * _lineHeight);
    _screen.println(thisLine);

    // If the line we just printed is shorter than what was there before, clear to EOL
    size_t lengthPrevious = strlen(_lineDataPrevious[_nextLine]);
    size_t lengthNew      = strlen(thisLine);

    if (lengthPrevious > lengthNew) {
      uint16_t left  = strlen(thisLine) * _charWidth;
      uint16_t top   = _nextLine * _lineHeight;
      uint16_t width = _width - (lengthNew * _charWidth);
      _screen.fillRect(left, top, width, _lineHeight, backColor);
    }

    // Save the new text for next time
    strncpy(_lineDataPrevious[_nextLine],
            thisLine, _charsPerLine);
  }
  _nextLine++;
}
