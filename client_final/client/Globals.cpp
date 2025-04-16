#include "Globals.h"

int currentPlayerId = 0;
int PLayersWaiting = 0;
int PLayersWaitingForTanks = 0;
int PLayersWaitingForSpomin = 0;
double tankAngle = 0;
double barrelAngle = 0;
Uint32 movementStartTime = 0;
Uint32 BoxSpawnTime = 20000; // 20000
Uint32 LastBox = 0;
std::random_device rd;
std::mt19937 gen(rd());
Uint32 powerUp = 10000;
unsigned int seed = 0;

int turnedColumn1 = -1;
int turnedRow1 = -1;
int turnedColumn2 = -1;
int turnedRow2 = -1;

bool fliped = 0;

float speed = 0;
float damage = 0;

std::string carSkin = "blue";
std::string tankSkin = "blue";

bool AvtiOver = 0;
bool TankiOver = 0;
bool SpominOver = 0;

