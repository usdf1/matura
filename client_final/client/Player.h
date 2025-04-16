#pragma once

#include "Vector2D.h"
#include "TextureManager.h"
#include <unordered_map>
#include "Collider.h"
#include <vector>

class Player {
private:
	int z;

public:
	Player(int id, float x, float y) {
		ID = id;
		Position.X = x;
		Position.Y = y;
		angle = 0;
		width = 32;
		height = 64;
		CrossedTheLine = 0;
		laps = 0;
		tank_width = 33; 
		tank_height = 33;
	}

	Player() { Position.X = 100; Position.Y = 100; 
	width = 32 * Game::GetInstance()->car_scale;
	height = 64 * Game::GetInstance()->car_scale;
	CrossedTheLine = 0;
	laps = 0;

	for (int i = 0; i < 3; i++) {
		checkpoints[i] = 0;
	}
	

	}

	int XPos() { return Position.X; }
	int YPos() { return Position.Y; }
	int GetID() { return ID; }
	Vector2D Position;
	float width, height;
	float tank_width, tank_height;
	int ID;
	double angle;
	double barrelAngle;
	float max;
	int laps;
	bool CrossedTheLine;
	int checkpoints[3];
	float tankSpeed = 5;
	float tankAngleSpeed = 0.9;

	bool shieldOn = 0;

	int state = 0;

	int CarS, TankS;

	std::string IsColliding;

	void Pospesi(Collider col);
	void Upocasni();
	void SetPosition(float x, float y, double ang, double Bangle, bool _shieldOn, int _state, int _car, int _tank) { Position.X = x; Position.Y = y; angle = ang; barrelAngle = Bangle; shieldOn = _shieldOn; state = _state; CarS = _car; TankS = _tank; };
	int GetAngle(int w_angle);
	void startMove(Collider col);
	void Draw(int ID);
	void DrawTanks(int ID);
	void Update(Collider col);
	void Clean();
	int AddLaps(Collider col);
	bool CheckCheckpoints(Collider col);
	void StartPosition();
	bool CheckCollisosWithPlayer(Player drugi);
	void resolveCollision(Player& other, float knockbackAmount);
	void NumSkins();
};

extern std::unordered_map<int, Player> players;