#include "application.h"

////////////////////////// INCLUDES ///////////////////////

#include "Adafruit_mfGFX/Adafruit_mfGFX.h"
#include "Adafruit_SSD1351_Photon/Adafruit_SSD1351_Photon.h"
#include "Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"
#include "Debounce/Debounce.h"

////////////////////////// DECLARATIONS ///////////////////

void setupDisplay();
void setup(void);
void loop();

///////////////////////// DEFINES /////////////////////////

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
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

////////////////////////// STRUCTS ///////////////////////////

struct lightNode {
  byte pwmAddress;
  byte tempAddress;
};


////////////////////////// GLOBALS ///////////////////////////

float globalBrightness = 1.0f;
float LEDTempCelsius = 20.0f;

bool displayMustUpdate = false;

// Controllers and stuff
Adafruit_SSD1351 display = Adafruit_SSD1351(spi_pin_cs, spi_pin_dc, spi_pin_rst);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Debounce modeButtonDebouncer = Debounce();

void debugPrint(char* text) {
  Serial.println(text);
}

void setupDisplay() {
  debugPrint("Setting up display");
  display.begin();

  display.fillScreen(BLACK);
  display.setCursor(0, 5);
  display.setTextColor(WHITE);
  display.setTextSize(1.5);
  display.println("Starting...");

  debugPrint("  Finished");
}

void setupLEDs() {

}

void setupControls() {
  // Setup the first button with an internal pull-up :
  pinMode(MODE_BUTTON_PIN,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  modeButtonDebouncer.attach(MODE_BUTTON_PIN);
  modeButtonDebouncer.interval(10); // interval in ms
}

void setupSensors() {

}

void setup(void) {
  Serial.begin(115200);

  setupDisplay();

  int delayLoops = 0;
  while ((!Serial) && (delayLoops<100)) {
    delay(10);
    delayLoops++;
  }

  setupLEDs();
  setupControls();
  setupSensors();
}

void loopSensors() {

}

void loopInputs() {

}

void loopLEDs() {

}

char * TimeToString(unsigned long t)
{
 static char str[12];
 long h = t / 3600;
 t = t % 3600;
 int m = t / 60;
 int s = t % 60;
 sprintf(str, "%04ld:%02d:%02d", h, m, s);
 return str;
}

void loopDisplay() {
  static unsigned int loopCount = 0;

  loopCount++;
  if (loopCount>100000) {
    displayMustUpdate = true;
    loopCount = 0;
  }
  if (displayMustUpdate) {
    const byte numChars = 64;
    static char modeName[numChars];
    static char brightness[numChars];
    static char temperature[numChars];
    static char loops[numChars];

    switch (0) {
      case 0: // Standard HSI mode.
        strcpy(modeName,"Mode: HSI");
        break;
      case 1: // Handle Strober mode.
        strcpy(modeName,"Mode: Stobe");
        break;
      case 2: // Fade HSI mode.
        strcpy(modeName,"Mode: Fade");
        break;
      case 3: // Standalone Cycling mode.
        strcpy(modeName,"Mode: Cycle");
        break;
      case 4: // Random LED shifting for art.
        strcpy(modeName,"Mode: Random");
        break;
      case 5: // Rotate among LEDs
        strcpy(modeName,"Mode: Rotate");
        break;
    }
/*
    char str_temp[6];
    dtostrf(globalBrightness * 100, 2, 0, str_temp);
    snprintf (brightness, numChars, "Brightness: %s", str_temp);
    display.drawStr(0,31,brightness);

    dtostrf(LEDTempCelsius, 2, 1, str_temp);
    snprintf (temperature, numChars, "Temp: %s c", str_temp);
    display.drawStr(0,46,temperature);

    display.sendBuffer();          // transfer internal memory to the display
*/

    display.fillScreen(BLACK);
    display.setCursor(0, 5);
    display.setTextColor(WHITE);
    display.setTextSize(1.5);
    display.println(TimeToString(millis()/1000));




  }
}

void loopControls() {
  static float brightness_EMA_a = 0.2;      //initialization of EMA alpha
  static int brightness_EMA_S = 1023 - analogRead(BRIGHTNESS_PIN);          //initialization of EMA S
  static int minBrightness = 100;
  static int maxBrightness = 900;

  modeButtonDebouncer.update();
  int modeButton = modeButtonDebouncer.read();
  if ( modeButtonDebouncer.fell() ) {
    //mode = static_cast<modes>((static_cast<int>(mode) + 1) % static_cast<int>(MODE_COUNT));
    displayMustUpdate = true;
  }

  int sensorValue = 1023 - analogRead(BRIGHTNESS_PIN);
  if (sensorValue > maxBrightness) {
    maxBrightness = sensorValue;
  }

  // record the minimum sensor value
  if (sensorValue < minBrightness) {
    minBrightness = sensorValue;
  }

  sensorValue = map(sensorValue, minBrightness, maxBrightness, 0, 1023);
  if (sensorValue < 50) {
    sensorValue = (sensorValue)/2;
    if (sensorValue < 25) {
      sensorValue = (sensorValue)/4;
      if (sensorValue < 10) {
        sensorValue = 0;
      }
    }
  }
  if (sensorValue > 987) {
    sensorValue = sensorValue + (sensorValue-987) * 2;
  }
  sensorValue = 100*constrain(sensorValue,0,1023);

  brightness_EMA_S = (brightness_EMA_a*sensorValue) + ((1-brightness_EMA_a)*brightness_EMA_S);    //run the EMA
  float newBrightness = brightness_EMA_S/(1023.0 * 100);

  if (newBrightness != globalBrightness) {
    globalBrightness = newBrightness;
    displayMustUpdate = true;
  }
}

void loop() {
  loopSensors();
  loopInputs();
  loopLEDs();
  loopDisplay();
}
