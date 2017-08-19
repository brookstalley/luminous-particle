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
// along with Luminous.  If not, see <http://www.gnu.org/licenses/>.
//
//**********************************************************
#ifndef PARTICLEFUNCTIONS_H
#define PARTICLEFUNCTIONS_H

#define PARTICLE_CONNECTION_TIMEOUT 30000

#include "Particle.h"

enum particleState { PARTICLE_DISCONNECTED, PARTICLE_CONNECTED };

enum wifiState { WIFI_DISCONNECTED, WIFI_CONNECTED };

extern particleState particleCurrentState;
extern particleState particleDesiredState;

extern wifiState wifiCurrentState;
extern LEDStatus luminousBooting;


void particleProcess();
void particleConnect();
void particleDisconnect();
void particleToggle();
void particleSetupFunctions();
void particleConnectionStarted();

#endif /* ifndef PARTICLEFUNCTIONS_H */
