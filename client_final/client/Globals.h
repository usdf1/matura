#pragma once
#include "SDL.h"
#include <random>

extern int currentPlayerId;
extern int PLayersWaiting;
extern int PLayersWaitingForTanks;
extern int PLayersWaitingForSpomin;
extern Uint32 movementStartTime;
extern float speed;
extern float damage;

extern double tankAngle;
extern double barrelAngle;
extern Uint32 BoxSpawnTime;
extern Uint32 LastBox;
extern std::random_device rd;
extern std::mt19937 gen;

extern Uint32 powerUp;

extern unsigned int seed;

extern int turnedColumn1;
extern int turnedRow1;
extern int turnedColumn2;
extern int turnedRow2;

extern bool fliped;

extern std::string carSkin;
extern std::string tankSkin;

extern bool AvtiOver;
extern bool TankiOver;
extern bool SpominOver;