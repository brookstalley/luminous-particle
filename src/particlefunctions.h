#ifndef PARTICLEFUNCTIONS_H
#define PARTICLEFUNCTIONS_H

#include "Particle.h"
#include "debug.h"

#define PARTICLE_CONNECTION_TIMEOUT 30000

enum particleState {PARTICLE_DISCONNECTED, PARTICLE_CONNECTED};

enum wifiState {WIFI_DISCONNECTED, WIFI_CONNECTED};

extern particleState particleCurrentState;
extern particleState particleDesiredState;

extern wifiState wifiCurrentState;
extern wifiState wifiDesiredState;

void particleProcess();
void particleConnect();
void particleDisconnect();
void particleToggle();

#endif
