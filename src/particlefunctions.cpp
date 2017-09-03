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
#include "particlefunctions.h"
#include "luminous-particle.h"

#include "ldebug.h"
#include "modes.h"

#include "Particle.h"

#include <string>

particleState particleCurrentState = PARTICLE_DISCONNECTED;
particleState particleDesiredState = PARTICLE_DISCONNECTED;

wifiState wifiCurrentState = WIFI_DISCONNECTED;
wifiState wifiDesiredState = WIFI_DISCONNECTED;

LEDStatus luminousBooting(RGB_COLOR_BLUE, LED_PATTERN_BLINK, LED_SPEED_FAST, LED_PRIORITY_IMPORTANT);

void particleDisconnect() {
  debugPrint(DEBUG_TRACE, "particleDisconnect: start");

  if (!Particle.connected() && (particleDesiredState == PARTICLE_DISCONNECTED)) {
    // We're already not connected, and we want to be disconnected.
    particleCurrentState = PARTICLE_DISCONNECTED;
  }

  if ((particleCurrentState == PARTICLE_CONNECTED) || (particleDesiredState == PARTICLE_CONNECTED)) {
    // We are either connected or in the process of connecting. Shut that down.
    debugPrint(DEBUG_MANDATORY, "particleDisconnect: Disconnecting from Particle cloud");
    Particle.disconnect();
    particleDesiredState = PARTICLE_DISCONNECTED;
    debugPrint(DEBUG_TRACE, "particleDisconnect: disconnecting");
    return;
  }
  debugPrint(DEBUG_TRACE, "particleDisconnect: end");
}

void particleConnect() {
  static unsigned long connectionStartMillis = 0;

  if (Particle.connected()) {
    // We're already connected, thanks, but maybe the state wasn't updated
    particleConnectionStarted();
    return;
  }

  // We are not connected, but the desired state is to be connected
  if (particleDesiredState == PARTICLE_CONNECTED) {
    if ((millis() - connectionStartMillis) > PARTICLE_CONNECTION_TIMEOUT) {
      // Timeout. Give up
      // TODO: build periodic reconnection attempt
      debugPrint(DEBUG_TRACE, "Timed out connecting to Particle cloud");
      particleDisconnect();
    }

    // Have not connected yet, but still within timeout
    return;
  }

  // We must be in PARTICLE_DISCONNECTED state, but someone called Connect()
  connectionStartMillis = millis();
  Particle.connect();
  particleDesiredState = PARTICLE_CONNECTED;
  debugPrint(DEBUG_MANDATORY, "particleConnect: Connecting to Particle cloud");
}

void particleToggle() {
  if (particleDesiredState == PARTICLE_CONNECTED) {
    debugPrint(DEBUG_TRACE, "particleToggle: disconnecting");
    particleDisconnect();
  } else {
    debugPrint(DEBUG_TRACE, "particleToggle: connecting");
    particleConnect();
  }
}

void particleProcess() {
  if (Particle.connected()) {
    // If we don't know we're connected, change that
    if (particleCurrentState == PARTICLE_DISCONNECTED) {
      debugPrint(DEBUG_MANDATORY, "particleProcess: Particle connection established");
      particleConnectionStarted();
    }
  } else {
    // We are not connected
    if (particleCurrentState == PARTICLE_CONNECTED) {
      // We think we're connected but Particle thinks otherwise
      particleCurrentState = PARTICLE_DISCONNECTED;

      if (particleDesiredState == PARTICLE_CONNECTED) {
        // ...but we WANT to be connected, so try to reconnect
        particleConnect();
      }
    }
  }

  // No matter what, run the process
  Particle.process();
}

int particleFunctionMode(String command) {
  debugPrintf(DEBUG_TRACE, "particleFunctionMode: starting for command %s", command);

  if (setModeByName(command)) {
    return 1;
  }
  return -1;
}

int particleFunctionBrightness(String command) {
  debugPrintf(DEBUG_MANDATORY, "particleFunctionBrightness: starting for command %s", command);
  int brightness = command.toInt();

  if ((brightness >= 0) && (brightness <= 100)) {
    globalBrightness     = ((float)brightness / 100.0f);
    lastBrightnessRemote = true;
    brightnessControl.setActivityThreshold(8.0f);
    return 1;
  }
  return -1;
}

void particleSetupFunctions() {
  debugPrint(DEBUG_MANDATORY, "particleSetupFunctions: starting");
  Particle.function("mode",       particleFunctionMode);
  Particle.function("brigntness", particleFunctionBrightness);
}

void particleConnectionStarted() {
  debugPrint(DEBUG_MANDATORY, "particleConnectionStarted: Particle connection established");
  particleCurrentState = PARTICLE_CONNECTED;
  particleSetupFunctions();
}
