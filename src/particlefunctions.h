#ifndef PARTICLEFUNCTIONS_H
#define PARTICLEFUNCTIONS_H

#define PARTICLE_CONNECTION_TIMEOUT 30000

enum particleState { PARTICLE_DISCONNECTED, PARTICLE_CONNECTED };

enum wifiState { WIFI_DISCONNECTED, WIFI_CONNECTED };

extern particleState particleCurrentState;
extern particleState particleDesiredState;

void particleProcess();
void particleConnect();
void particleDisconnect();
void particleToggle();
void particleSetupFunctions();
void particleConnectionStarted();

#endif /* ifndef PARTICLEFUNCTIONS_H */
