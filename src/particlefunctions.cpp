#include "particlefunctions.h"

particleState particleCurrentState = PARTICLE_DISCONNECTED;
particleState particleDesiredState = PARTICLE_DISCONNECTED;

wifiState wifiCurrentState = WIFI_DISCONNECTED;
wifiState wifiDesiredState = WIFI_DISCONNECTED;


void particleDisconnect() {
  debugPrint(DEBUG_TRACE, "particleDisconnect: start");;

  if (!Particle.connected() && (particleDesiredState == PARTICLE_DISCONNECTED)) {
    // We're already not connected, and we want to be disconnected.
    particleCurrentState = PARTICLE_DISCONNECTED;
  }

  if ((particleCurrentState == PARTICLE_CONNECTED) || (particleDesiredState == PARTICLE_CONNECTED)) {
    // We are either connected or in the process of connecting. Shut that down.
    debugPrint(DEBUG_MANDATORY, "particleDisconnect: Disconnecting from Particle cloud");
    Particle.disconnect();
    particleDesiredState = PARTICLE_DISCONNECTED;
    debugPrint(DEBUG_TRACE, "particleDisconnect: disconnecting");;
    return;
  }
  debugPrint(DEBUG_TRACE, "particleDisconnect: end");;
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
      debugPrint(DEBUG_TRACE, "Timed out connecting to Particle cloud");;
      particleDisconnect();
    }

    // Have not connected yet, but still within timeout
    return;
  }

  // We must be in PARTICLE_DISCONNECTED state, but someone called Connect()
  connectionStartMillis = millis();
  Particle.connect();
  particleDesiredState = PARTICLE_CONNECTED;
  debugPrint(DEBUG_MANDATORY, "particleConnect: Connecting to Particle cloud");;
}

void particleToggle() {
  if (particleDesiredState == PARTICLE_CONNECTED) {
    debugPrint(DEBUG_TRACE, "particleToggle: disconnecting");;
    particleDisconnect();
  } else {
    debugPrint(DEBUG_TRACE, "particleToggle: connecting");;
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
  debugPrintf(DEBUG_MANDATORY, "particleFunctionMode: starting for command %s", command);

  for (uint16_t i = 0; i < modeCount; i++) {
    if (command == modes[i]->getName()) {
      modes[currentMode]->end(allLights);
      currentMode = i;
      modes[currentMode]->start(allLights);
      return 1;
    }
  }
  return -1;
}

void particleSetupFunctions() {
  debugPrint(DEBUG_MANDATORY, "particleSetupFunctions: starting");
  Particle.function("mode", particleFunctionMode);
}

void particleConnectionStarted() {
  debugPrint(DEBUG_MANDATORY, "particleConnectionStarted: Particle connection established");
  particleCurrentState = PARTICLE_CONNECTED;
  particleSetupFunctions();
}
