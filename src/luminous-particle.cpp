#include "application.h"

////////////////////////// INCLUDES ///////////////////////

#include "Adafruit-GFX-library/Adafruit_GFX.h"
#include "Adafruit_SSD1351_Photon.h"
#include "PCA9685-Particle/PCA9685.h"
#include "clickButton/clickButton.h"
//#include "E131/E131.h"

#include "emitter.h"
#include "hsicolor.h"
#include "compositelight.h"
#include "hsilamp.h"
#include "debug.h"

////////////////////////// DECLARATIONS ///////////////////

void setupDisplay();
void setup(void);
void loop();
void modeOff();
void modeTest();
void modeE131();

///////////////////////// DEFINES /////////////////////////

// Debugging
#define PCA9685_ENABLE_DEBUG_OUTPUT

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


////////////////////////// STRUCTS ///////////////////////////


typedef void (* luminousFunctionPointer) ();

typedef struct {
  const char name[10];
  luminousFunctionPointer functionPointer;
} luminousMode ;

const int modeCount = 3;

luminousMode modes[modeCount] = {
  {"Off", modeOff},
  {"Test", modeTest},
  {"E131", modeE131}
};

int currentMode = 0;

////////////////////////// GLOBALS ///////////////////////////

float globalBrightness = 1.0f;
float LEDTempCelsius = 20.0f;

bool displayMustUpdate = false;
bool lightsMustUpdate = false;

////////////////////////// Controllers and stuff
Adafruit_SSD1351 display = Adafruit_SSD1351(spi_pin_cs, spi_pin_dc, spi_pin_rst);
PCA9685 pwm = PCA9685(Wire);
ClickButton modeButton = ClickButton(MODE_BUTTON_PIN, LOW,  CLICKBTN_PULLUP);

// Shared lights

Emitter emitterLZ7white("LZ7-w",0.202531646, 0.469936709, (float)180/180);
Emitter emitterLZ7red("LZ7-r",0.5137017676, 0.5229440531, (float)78/78);
Emitter emitterLZ7amber("LZ7-a",0.3135687079, 0.5529418124, (float)60/60);
Emitter emitterLZ7green("LZ7-g",0.0595846867, 0.574988823, (float)125/125);
Emitter emitterLZ7cyan("LZ7-c",0.0306675939, 0.5170937486, (float)95/95);
Emitter emitterLZ7blue("LZ7-b",0.1747943747, 0.1117834986, (float)30/30);
Emitter emitterLZ7violet("LZ7-v",0.35, 0.15, (float)30/30);

// Standard luminous node & wiring
CompositeLight LZ7(emitterLZ7white, 5);

HSILamp testnode(LZ7, pwm, (uint8_t)0);

//////////////// MODES ///////////////////////////////////
void modeOff() {

}

void effectTest() {
  const unsigned int millisPerColor = 2000;
  static unsigned long firstChange = millis();
  static unsigned int counter = 0;
  static unsigned int lastCounter = 0;
  static char msg[200];

  counter = floor((millis() - firstChange) / millisPerColor);

  //sprintf(msg,"millis %d, firstchange %d, millisperColor %u, counter %d",millis(),firstChange,millisPerColor,counter);
  //debugPrint(msg);
  if (lightsMustUpdate || (counter != lastCounter)) {
    testnode.setSingleEmitterOn(counter);
    lastCounter = counter;
  }
}

void modeTest() {
  effectTest();
}

void modeE131() {

}


// Actual nodes
// BREAKS
//HSILamp testnode(LZ7, pwm, (uint8_t)0);
/*
// Our lists
std::vector<HSILamp> allNodes {testnode};
*/
////////////////////////// MAIN ////////////////////////////


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
  debugPrint("Setting up LEDs");

  LZ7.addEmitter(emitterLZ7red, 0);
  LZ7.addEmitter(emitterLZ7amber, 3);
  LZ7.addEmitter(emitterLZ7green, 1);
  LZ7.addEmitter(emitterLZ7cyan, 4);
  LZ7.addEmitter(emitterLZ7blue, 2);
  LZ7.addEmitter(emitterLZ7violet, 6);

  debugPrint("Setting up lamps");

  testnode.begin();

  debugPrint("Setting up outputs");
  Wire.begin();                       // Wire must be started first
  Wire.setClock(400000);              // Supported baud rates are 100kHz, 400kHz, and 1000kHz
  debugPrint("  resetDevices");
  pwm.resetDevices();
  debugPrint("  init");
  pwm.init(B000000, PCA9685_MODE_OUTPUT_ONACK | PCA9685_MODE_OUTPUT_TPOLE);
  debugPrint("  setPWMFrequency");
  pwm.setPWMFrequency(400);
  debugPrint("Done setting up LEDs");

}

void setupControls() {
  // Setup the first button with an internal pull-up :
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  modeButton.debounceTime   = 20;   // Debounce timer in ms
  modeButton.multiclickTime = 250;  // Time limit for multi clicks
  modeButton.longClickTime  = 750; // time until "held-down clicks" register

}

void setupSensors() {

}

void setup(void) {
  setDebugOutput(true);
  Serial.begin(9600);

  setupDisplay();

  int delayLoops = 0;
  while ((!Serial) && (delayLoops<300)) {
    delay(10);
    delayLoops++;
  }
  if (Serial)
    debugPrint("Serial came up");

  setupLEDs();
  setupControls();
  setupSensors();
}

void effectOff() {

}

void loopSensors() {

}

void loopInputs() {

}

void loopLEDs() {
 //effectTest();
 modes[currentMode].functionPointer();
 lightsMustUpdate = false;
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
  const unsigned long maxUpdateLagMillis = 500;
  static unsigned long lastUpdateMillis = 0;

  const byte numChars = 64;
  static char brightnessReadout[numChars];
  static char debugReadout[numChars];

  if (millis() - lastUpdateMillis > maxUpdateLagMillis) {
    displayMustUpdate = true;
  }

  if (displayMustUpdate) {

    //static char temperature[numChars];

    sprintf (brightnessReadout, "Brightness: %2.1f", globalBrightness * 100);
    sprintf (debugReadout, (getDebugOutput() ? "Debug: serial" : "Debug: off"));
/*
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
    display.println(modes[currentMode].name);
    display.println(brightnessReadout);
    display.println(debugReadout);

    lastUpdateMillis = millis();
    displayMustUpdate = false;


  }
}

void loopControlBrightness() {
  static float brightness_EMA_a = 0.2;      //initialization of EMA alpha
  static int brightness_EMA_S = 1023 - analogRead(BRIGHTNESS_PIN);          //initialization of EMA S
  static int minBrightness = 100;
  static int maxBrightness = 900;

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

  float brightnessChange = newBrightness - globalBrightness;


  if (abs(brightnessChange) >= 0.001) {
    char msg[100];
    sprintf(msg,"New brightness: %f (change %f)",newBrightness,brightnessChange);
    debugPrint(msg);
    //displayMustUpdate = true;
    lightsMustUpdate = true;
  }


  globalBrightness = newBrightness;


}

void loopControls() {
  static int modeClicks = 0;
  modeButton.Update();
  modeClicks = modeButton.clicks;
  if (modeButton.clicks != 0) {

    if ( modeClicks == 1 ) {
      debugPrint("One click");
      currentMode = ((currentMode + 1) % modeCount);
      displayMustUpdate = true;
      lightsMustUpdate = true;
    }

    if ( modeClicks == -1 ) {
      debugPrint("One long click");
      setDebugOutput(!getDebugOutput());
      displayMustUpdate = true;
    }
  }

  loopControlBrightness();

}

void loop() {
  //loopSensors();
  loopControls();
  //loopInputs();
  loopLEDs();
  loopDisplay();
}
