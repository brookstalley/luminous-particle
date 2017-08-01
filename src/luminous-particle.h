#ifndef LUMINOUS_PARTICLE_H
#define LUMINOUS_PARTICLE_H

#include "HSILight.h"
#include "ResponsiveAnalogRead/ResponsiveAnalogRead.h"

#include <vector>
#include <memory>

#define DEBUG_BUILD

#define spi_pin_sclk A3
#define spi_pin_mosi A5
#define spi_pin_dc   D7
#define spi_pin_cs   A2
#define spi_pin_rst  D5

#define i2c_pin_scl D1
#define i2c_pin_sda D0

#define TEMP_PIN A1
#define BRIGHTNESS_PIN  A0
#define MODE_BUTTON_PIN D4

extern float globalBrightness;
extern bool  lastBrightnessRemote;

extern float LEDTempCelsius;

extern bool displayMustUpdate;
extern bool lightsMustUpdate;

extern ResponsiveAnalogRead brightnessControl;
extern std::vector < std::shared_ptr < HSILight >> allLights;

void setupDisplay();
void setup(void);
void loop();
void modeOff();
void modeTest();
void modeRotate();
void modeE131();

#endif /* ifndef LUMINOUS_PARTICLE_H */
