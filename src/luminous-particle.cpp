#include "application.h"
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

#include <math.h>

////////////////////////// INCLUDES ///////////////////////

#include "Adafruit-GFX-library/Adafruit_GFX.h"
#include "Adafruit_SSD1351_Photon.h"
#include "clickButton/clickButton.h"

// #include "E131/E131.h"

#include "emitter.h"
#include "hsicolor.h"
#include "compositemodule.h"
#include "hsilight.h"
#include "debug.h"
#include "modes.h"
#include "outputPCA9685.h"
#include "particlefunctions.h"

////////////////////////// DECLARATIONS ///////////////////

void setupDisplay();
void setup(void);
void loop();
void modeOff();
void modeTest();
void modeRotate();
void modeE131();

///////////////////////// DEFINES /////////////////////////

// Debugging
#define DEBUG_BUILD

// You can use any (4 or) 5 pins
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

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

////////////////////////// GLOBALS ///////////////////////////

float globalBrightness = 1.0f;
float LEDTempCelsius   = 20.0f;

bool displayMustUpdate = false;
bool lightsMustUpdate  = false;

////////////////////////// Controllers and stuff
Adafruit_SSD1351 display    = Adafruit_SSD1351(spi_pin_cs, spi_pin_dc, spi_pin_rst);
OutputPCA9685    mainOutput = OutputPCA9685(Wire, 0x40);
ClickButton modeButton      = ClickButton(MODE_BUTTON_PIN, LOW,  CLICKBTN_PULLUP);

// Shared lights

Emitter emitterLZ7white("LZ7-w", 0.202531646, 0.469936709, (float)180 / 180);
Emitter emitterLZ7red("LZ7-r", 0.5137017676, 0.5229440531, (float)78 / 78);
Emitter emitterLZ7amber("LZ7-a", 0.3135687079, 0.5529418124, (float)60 / 60);
Emitter emitterLZ7green("LZ7-g", 0.0595846867, 0.574988823, (float)125 / 125);
Emitter emitterLZ7cyan("LZ7-c", 0.0306675939, 0.5170937486, (float)95 / 95);
Emitter emitterLZ7blue("LZ7-b", 0.1747943747, 0.1117834986, (float)30 / 30);
Emitter emitterLZ7violet("LZ7-v", 0.35, 0.15, (float)30 / 30);

// Standard luminous node & wiring
CompositeModule LZ7;

// Create a light that has an LZ7 module, on our main output at address 0
// Eventually we'll have different groups of lights, so just make a group
// of one because the various effects expect to operate on a group.
std::vector<std::shared_ptr<HSILight> > allLights = {
  std::make_shared<HSILight>(HSILight("Light1", LZ7, mainOutput, (uint16_t)0))
};

////////////////////////// MAIN ////////////////////////////


void setupDisplay() {
  debugPrint(DEBUG_TRACE, "Setting up display");;
  display.begin();

  display.fillScreen(BLACK);
  display.setCursor(0, 5);
  display.setTextColor(WHITE);
  display.setTextSize(1.5);
  display.println("Starting...");

  debugPrint(DEBUG_TRACE, "  Finished");;
}

void setupLEDs() {
  debugPrint(DEBUG_TRACE, "Setting up outputs");
  Wire.begin();          // Wire must be started first
  Wire.setClock(400000); // Supported baud rates are 100kHz, 400kHz, and 1000kHz

  mainOutput.init();

  debugPrint(DEBUG_TRACE, "Setting up LEDs");

  LZ7.addWhiteEmitter(emitterLZ7white, 5);
  LZ7.addColorEmitter(emitterLZ7red, 0);
  LZ7.addColorEmitter(emitterLZ7amber, 3);
  LZ7.addColorEmitter(emitterLZ7green, 1);
  LZ7.addColorEmitter(emitterLZ7cyan, 4);
  LZ7.addColorEmitter(emitterLZ7blue, 2);
  LZ7.addColorEmitter(emitterLZ7violet, 6);

  debugPrint(DEBUG_TRACE, "Setting up lamps");

  std::for_each(allLights.begin(), allLights.end(), [&](std::shared_ptr<HSILight>light) {
    light->begin();
  });

  debugPrint(DEBUG_TRACE, "Done setting up LEDs");
}

void setupControls() {
  // Setup the first button with an internal pull-up :
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  modeButton.debounceTime   = 20;  // Debounce timer in ms
  modeButton.multiclickTime = 250; // Time limit for multi clicks
  modeButton.longClickTime  = 750; // time until "held-down clicks" register
}

void setupSensors() {}

void setup(void) {
  setDebugLevel(DEBUG_TRACE);
  Serial.begin(9600);
  delay(100);
  debugPrint(DEBUG_MANDATORY, "Starting...");

  setupDisplay();

  int delayLoops = 0;

  while ((!Serial) && (delayLoops < 300)) {
    delay(10);
    delayLoops++;
  }

  if (Serial) debugPrint(DEBUG_TRACE, "Serial came up");

  setupLEDs();
  setupControls();
  setupSensors();
}

void loopSensors() {}

void loopInputs()  {}

void loopLEDs() {
  modes[currentMode]->run(allLights, lightsMustUpdate);
  lightsMustUpdate = false;
}

char* TimeToString(unsigned long t)
{
  static char str[12];
  long h = t / 3600;

  t = t % 3600;
  int m = t / 60;
  int s = t % 60;
  sprintf(str, "%02ld:%02d:%02d", h, m, s);
  return str;
}

void loopDisplay() {
  // Assumes monospace fonts
  const uint8_t displayWidth             = 128;
  const unsigned long maxUpdateLagMillis = 500;
  const uint8_t lineHeight               = 9;
  const uint8_t charWidth                = 6;
  const uint8_t charsPerLine             = floor(displayWidth / charWidth);
  const uint8_t linesToDisplay           = 6;

  static unsigned long lastUpdateMillis = 0;

  static char lineData[linesToDisplay][charsPerLine + 1]         = {};
  static char lineDataPrevious[linesToDisplay][charsPerLine + 1] = {};

  if (millis() - lastUpdateMillis > maxUpdateLagMillis) {
    displayMustUpdate = true;
  }

  if (displayMustUpdate) {
    display.setTextColor(WHITE, BLACK);

    char debugName[12];
    getDebugLevelName(getDebugLevel(), debugName, sizeof(debugName));

    sprintf(lineData[0], "Running:    %s", TimeToString(millis() / 1000));
    sprintf(lineData[1], "Mode:       %s", modes[currentMode]->getName());
    sprintf(lineData[2], "Brightness: %2.0f%%", globalBrightness * 100);
    sprintf(lineData[3], "Debug:      %s", debugName);
    strcpy(lineData[4], (particleCurrentState == PARTICLE_CONNECTED ?
                         "Particle:   online" :
                         "Particle:   offline"));

    strcpy(lineData[5], (particleDesiredState != particleCurrentState ?
                         (particleDesiredState == PARTICLE_CONNECTED ?
                          "  Connecting" : "  Disconnecting")
                         : ""));

    /*
        dtostrf(LEDTempCelsius, 2, 1, str_temp);
        snprintf (temperature, numChars, "Temp: %s c", str_temp);
        display.drawStr(0,46,temperature);

        display.sendBuffer();          // transfer internal memory to the
           display
     */

    for (uint8_t i = 0; i < linesToDisplay; i++) {
      // We always have 12 characters of label
      // So, clear the space from char 13 until the end (currently hardcoded at
      // 128)
      if (strcmp(lineData[i], lineDataPrevious[i]) != 0) {
        // Print our new text
        display.setCursor(0, i * lineHeight);
        display.println(lineData[i]);

        // Determine how much we need to clear after the next text
        uint16_t charsBeforeEOL = strlen(lineData[i]);

        size_t lengthPrevious = strlen(lineDataPrevious[i]);
        size_t lengthNew      = strlen(lineData[i]);

        if (lengthPrevious > lengthNew) {
          uint16_t left   = charsBeforeEOL * charWidth;
          uint16_t top    = i * lineHeight;
          uint16_t width  = displayWidth - (lengthNew * charWidth);
          uint16_t height = lineHeight;
          display.fillRect(left, top, width, height, BLACK);

          // setDebugLevel(DEBUG_INSANE); // because otherwise we won't see this
          // output
          // debugPrintf(DEBUG_INSANE, "updateDisplay: previous (%u) longer than
          // new (%u). Filling %u,%u for w=%u, h=%u",
          //            lengthPrevious, lengthNew, left, top, width, height);
        }

        // Save the new text for next time
        strcpy(lineDataPrevious[i], lineData[i]);
      }
    }
    lastUpdateMillis  = millis();
    displayMustUpdate = false;
  }
}

void loopControlBrightness() {
  static float brightness_EMA_a = 0.7;
  static int   brightness_EMA_S = 1023 - analogRead(BRIGHTNESS_PIN);
  static int   minBrightness    = 100;
  static int   maxBrightness    = 900;
  int sensorValue               = 1023 - analogRead(BRIGHTNESS_PIN);

  if (sensorValue > maxBrightness) {
    maxBrightness = sensorValue;
  }

  // record the minimum sensor value
  if (sensorValue < minBrightness) {
    minBrightness = sensorValue;
  }

  sensorValue = map(sensorValue, minBrightness, maxBrightness, 0, 1023);

  if (sensorValue < 25) {
    sensorValue = (sensorValue) / 2;

    if (sensorValue < 10) {
      sensorValue = (sensorValue) / 4;

      if (sensorValue < 5) {
        sensorValue = 0;
      }
    }
  }

  if (sensorValue > 1000) {
    sensorValue = sensorValue + (sensorValue - 1000) * 2;
  }
  sensorValue      = 100 * constrain(sensorValue, 0, 1023);
  brightness_EMA_S = (brightness_EMA_a * sensorValue) + ((1 - brightness_EMA_a) * brightness_EMA_S);
  float newBrightness    = brightness_EMA_S / (1023.0 * 100);
  float brightnessChange = newBrightness - globalBrightness;

  if (abs(brightnessChange) >= 0.002) {
    lightsMustUpdate  = true;
    displayMustUpdate = true;
  }
  globalBrightness = newBrightness;
}

void loopControls() {
  static int modeClicks = 0;

  modeButton.Update();
  modeClicks = modeButton.clicks;

  if (modeButton.clicks != 0) {
    if (modeClicks == 1) {
      debugPrint(DEBUG_TRACE, "Click: change mode");
      nextMode();
      displayMustUpdate = true;
      lightsMustUpdate  = true;
    }

    if (modeClicks == 2) {
      debugPrint(DEBUG_TRACE, "Double-click: toggle Particle cloud");
      particleToggle();
      displayMustUpdate = true;
    }

    if (modeClicks == 3) {
      debugPrint(DEBUG_TRACE, "Triple-click: setup particle functions");
      particleSetupFunctions();
    }

    if (modeClicks == -1) {
      debugPrint(DEBUG_TRACE, "Long click: increment debugging");
      setDebugLevel(getDebugLevel() + 1);
      displayMustUpdate = true;
    }
  }

  loopControlBrightness();
}

void loop() {
  // loopSensors();
  loopControls();

  // loopInputs();
  loopLEDs();
  particleProcess();
  loopDisplay();
  particleProcess();
}

void connect() {
  particleConnectionStarted();
}
