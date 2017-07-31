#ifndef PARTICLEFUNCTIONS_H
#define PARTICLEFUNCTIONS_H

#include "Particle.h"
#include "debug.h"
#include "modes.h"

#define PARTICLE_CONNECTION_TIMEOUT 30000

enum particleState { PARTICLE_DISCONNECTED, PARTICLE_CONNECTED };

enum wifiState { WIFI_DISCONNECTED, WIFI_CONNECTED };

extern particleState particleCurrentState;
extern particleState particleDesiredState;

extern wifiState wifiCurrentState;
extern wifiState wifiDesiredState;

extern std::vector < std::shared_ptr < HSILight >> allLights;

void particleProcess();
void particleConnect();
void particleDisconnect();
void particleToggle();
void particleSetupFunctions();
void particleConnectionStarted();

#endif /* ifndef PARTICLEFUNCTIONS_H */
