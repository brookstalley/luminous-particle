//*********************************************************
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
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//
//**********************************************************
#include "luminous-particle.h"
#include "emitter.h"
#include "hsicolor.h"
#include "compositemodule.h"
#include "hsilight.h"
#include "ldebug.h"
#include "modes.h"
#include "credentials.h"

#include "outputPCA9685.h"
#include "temperatureAds1115.h"
#include "particlefunctions.h"

#include "Adafruit-GFX-library/Adafruit_GFX.h"
#include "Adafruit_SSD1351_Photon.h"
#include "clickButton/clickButton.h"
#include "ResponsiveAnalogRead/ResponsiveAnalogRead.h"
#include "E131/E131.h"

#include "application.h"

#include <math.h>
#include <algorithm>

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

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

float globalBrightness     = 1.0f;
bool lastBrightnessRemote = false;

float LEDTempCelsius = 20.0f;

bool displayMustUpdate = false;
bool lightsMustUpdate  = false;

unsigned int loopsPerSecond = 0.0f;

////////////////////////// Controllers and stuff
Adafruit_SSD1351 display(spi_pin_cs, spi_pin_dc, spi_pin_rst);
ClickButton modeButton(MODE_BUTTON_PIN, LOW,  CLICKBTN_PULLUP);
ResponsiveAnalogRead brightnessControl(BRIGHTNESS_PIN, true);

// Input and output devices

std::shared_ptr<OutputPCA9685> mainOutput =
	std::make_shared<OutputPCA9685>(Wire, 0x40);

std::shared_ptr<TemperatureAds1115> mainTemperature
        = std::make_shared<TemperatureAds1115>(Wire, 0x48, 10000, 10000, 25, 3950);

std::shared_ptr<E131> mainUniverse = std::make_shared<E131>();

// Shared lights with coordinates in CIE LUV colorspace
Emitter emitterLZ7white("LZ7-w", 0.202531646, 0.469936709, (float)180 / 180);
Emitter emitterLZ7red("LZ7-r", 0.5137017676, 0.5229440531, (float)78 / 78);
Emitter emitterLZ7amber("LZ7-a", 0.3135687079, 0.5529418124, (float)60 / 60);
Emitter emitterLZ7green("LZ7-g", 0.0595846867, 0.574988823, (float)125 / 125);
Emitter emitterLZ7cyan("LZ7-c", 0.0306675939, 0.5170937486, (float)95 / 95);
Emitter emitterLZ7blue("LZ7-b", 0.1747943747, 0.1117834986, (float)30 / 30);
Emitter emitterLZ7violet("LZ7-v", 0.35, 0.15, (float)30 / 30);

// LEDEngin LZ7 that should be dimmed above 70c and turned off above 90c
CompositeModule LZ7(70.f, 90.0f);

// Create a light that has an LZ7 module, on our main output at address 0
// Eventually we'll have different groups of lights, so just make a group
// of one because the various effects expect to operate on a group.
std::vector<std::shared_ptr<HSILight> > allLights = {
	std::make_shared<HSILight>(
		HSILight("Light1", LZ7, mainOutput,
		         (uint16_t)0,
		         mainUniverse,
		         (uint16_t)1,
		         mainTemperature, (uint16_t)0)
		)
};

////////////////////////// MAIN ////////////////////////////

void setupDisplay() {
	debugPrint(DEBUG_TRACE, "setupDisplay: starting");
	display.begin();

	display.fillScreen(BLACK);
	display.setCursor(0, 5);
	display.setTextColor(WHITE);
	display.setTextSize(1);
	display.println("Starting...");

	debugPrint(DEBUG_TRACE, "  Finished");
}

void setupLEDs() {
	debugPrint(DEBUG_TRACE, "setupLEDs: starting");
	Wire.begin();    // Wire must be started first
	Wire.setClock(400000); // Supported baud rates are 100kHz, 400kHz, and 1000kHz

	mainOutput->init();

	LZ7.addWhiteEmitter(emitterLZ7white, 5);
	LZ7.addColorEmitter(emitterLZ7red,    0);
	LZ7.addColorEmitter(emitterLZ7amber,  3);
	LZ7.addColorEmitter(emitterLZ7green,  1);
	LZ7.addColorEmitter(emitterLZ7cyan,   4);
	LZ7.addColorEmitter(emitterLZ7blue,   2);
	LZ7.addColorEmitter(emitterLZ7violet, 6);

	LZ7.calculate();

	debugPrint(DEBUG_TRACE, "  setting up lamps");

	mainTemperature->begin();

	std::for_each(allLights.begin(), allLights.end(),
	              [&](std::shared_ptr<HSILight>light) {
		light->begin();
	});

	debugPrint(DEBUG_TRACE, "  Setting initial mode");
	setModeByNumber(0);
	debugPrint(DEBUG_TRACE, "  Finished");
}

void setupControls() {
	// Setup the first button with an internal pull-up :
	pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
	modeButton.debounceTime   = 20;// Debounce timer in ms
	modeButton.multiclickTime = 250; // Time limit for multi clicks
	modeButton.longClickTime  = 750;// time until "held-down clicks" register
	// Setup the brightness control
	lastBrightnessRemote = false;
	brightnessControl.enableEdgeSnap();
	brightnessControl.setAnalogResolution(4096);
}

void setupSensors() {
}

void setupE131() {
	debugPrint(DEBUG_TRACE, "setupE131: Starting");

	if (!WiFi.ready()) {
		wifiCurrentState = WIFI_DISCONNECTED;
		debugPrint(DEBUG_ERROR, "setupE131: WiFi not connected");
		return;
	}
	mainUniverse->begin(10);
	// It is insane that the following line does not compile.
	//debugPrintf(DEBUG_TRACE, "  server=%s:%u", WiFi.localIP(), mainUniverse->getUdpPort());
	IPAddress localAddr = WiFi.localIP();
	byte oct1 = localAddr[0];
	byte oct2 = localAddr[1];
	byte oct3 = localAddr[2];
	byte oct4 = localAddr[3];
	char ipChars[16];
	sprintf(ipChars, "%d.%d.%d.%d", oct1, oct2, oct3, oct4);
	debugPrintf(DEBUG_TRACE, "  server=%s", ipChars);
}

void setupNetwork() {
	debugPrint(DEBUG_TRACE, "setupNetwork: starting");
	WiFi.on();
	WiFi.disconnect();
	wifiCurrentState = WIFI_DISCONNECTED;
	WiFi.clearCredentials();
	// SSID and password are stored in credentials.h
	WiFi.setCredentials(WIFI_SSID, WIFI_PASSWORD);
	WiFi.connect();
	debugPrintf(DEBUG_TRACE,
	            "  connecting to %s with password %s",
	            WIFI_SSID,
	            WIFI_PASSWORD);

	for (uint16_t i = 0; (wifiCurrentState != WIFI_CONNECTED) && (i < 30);
	     i++) {
		waitFor(WiFi.ready, 1000);

		if (WiFi.ready()) {
			wifiCurrentState = WIFI_CONNECTED;
			debugPrint(DEBUG_TRACE, "  connected");
			return;
		} else {

			debugPrintf(DEBUG_TRACE, "  waiting for connection to %s:%s (%u)", WIFI_SSID, WIFI_PASSWORD, i);
		}
	}
}

void setup(void) {
	luminousBooting.setActive(true);
	setDebugLevel(DEBUG_TRACE);
	Serial.begin(9600);

	// Give serial up to 3 seconds to come up
	int delayLoops = 0;

	while ((!Serial) && (delayLoops < 300)) {
		delay(10);
		delayLoops++;
	}

	// No sense logging before we start serial or network
	debugPrint(DEBUG_MANDATORY, "Starting...");
	luminousBooting.setActive(false);

	setupDisplay();
	setupNetwork();
	setupLEDs();
	setupControls();
	setupSensors();
	setupE131();
	debugPrint(DEBUG_TRACE, "  Finished");
}

void loopSensors() {
}

void loopInputs()  {
}

void loopLEDs() {
	runCurrentMode();
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
	const uint8_t displayHeight            = 128;
	const unsigned long maxUpdateLagMillis = 500;
	const uint8_t lineHeight         = 9;
	const uint8_t charWidth          = 6;
	const uint8_t charsPerLine       = floor(displayWidth / charWidth);
	const uint8_t maxLines           = (displayHeight / lineHeight);

	static unsigned long lastUpdateMillis = 0;

	static char lineData[maxLines][charsPerLine + 1]         = {};
	static char lineDataPrevious[maxLines][charsPerLine + 1] = {};

	if (millis() - lastUpdateMillis > maxUpdateLagMillis) {
		displayMustUpdate = true;
	}

	if (!displayMustUpdate) return;


	char debugName[12];
	uint8_t currentLine = 0;
	getDebugLevelName(getDebugLevel(), debugName, sizeof(debugName));

	sprintf(lineData[currentLine++], "Running:    %s",
	        TimeToString(millis() / 1000));
	sprintf(lineData[currentLine++], "Mode:       %s", getCurrentModeName());
	sprintf(lineData[currentLine++], "Loops/s:    %u",
	        loopsPerSecond);
	if (lastBrightnessRemote) {
		sprintf(lineData[currentLine++], "Brightness: [%2.0f%%]",
		        globalBrightness * 100);
	} else {
		sprintf(lineData[currentLine++],
		        "Brightness: %2.0f%%",
		        globalBrightness * 100);
	}

	// Hack for now to show first temperature
	sprintf(lineData[currentLine++],
	        "Temp:       %3.0f",
	        allLights[0]->getTemperature());
	sprintf(lineData[currentLine++], "Debug:      %s", debugName);

	if (wifiCurrentState == WIFI_CONNECTED) {
		sprintf(lineData[currentLine++], "WiFi:       %s", WiFi.SSID());
	} else {
		strcpy(lineData[currentLine++], "WiFi:       offline");
	}

	strcpy(lineData[currentLine++], (particleCurrentState == PARTICLE_CONNECTED ?
	                                 "Particle:   online" :
	                                 "Particle:   offline"));

	strcpy(lineData[currentLine++], (particleDesiredState != particleCurrentState ?
	                                 (particleDesiredState == PARTICLE_CONNECTED ?
	                                  "  Connecting" : "  Disconnecting")
					 : ""));

	display.setTextColor(WHITE, BLACK);

	for (uint8_t i = 0; i < currentLine; i++) {
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
			}

			// Save the new text for next time
			strcpy(lineDataPrevious[i], lineData[i]);
		}
	}
	lastUpdateMillis  = millis();
	displayMustUpdate = false;
}

void loopControlBrightness() {
	brightnessControl.update();
	if (!brightnessControl.hasChanged()) return;

	globalBrightness = ((float)(4095 - brightnessControl.getValue()) / 4095.0f);

	if (lastBrightnessRemote) {
		// The previous change was remote; change the indicator and reset the
		// sensitivity because the remote change turned it down
		lastBrightnessRemote = false;
		brightnessControl.setActivityThreshold(4.0f);
	}
	lightsMustUpdate  = true;
	displayMustUpdate = true;
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

void loopE131() {
	if (wifiCurrentState == WIFI_CONNECTED) {
		if (!WiFi.ready()) {
			wifiCurrentState = WIFI_DISCONNECTED;
			debugPrint(DEBUG_ERROR, "loopE131: WiFi not connected");
			return;
		}
		uint16_t dataCount = mainUniverse->parsePacket();
		debugPrintf(DEBUG_INSANE, "loopE131: got %u packets", dataCount);
	}

	// TODO: Reconnect logic?
}

void loopTimer() {
	static unsigned long lastMillis = millis();
	static unsigned int loops = 0;

	loops++;
	if (millis() - lastMillis > 1000) {
		lastMillis = millis();
		loopsPerSecond = loops;
		loops = 0;
	}
}

void loop() {
	// loopSensors();
	loopTimer();
	particleProcess();
	loopControls();
	particleProcess();
	loopE131();
	particleProcess();
	loopLEDs();
	particleProcess();
	loopDisplay();
	particleProcess();
}

void connect() {
	particleConnectionStarted();
}
