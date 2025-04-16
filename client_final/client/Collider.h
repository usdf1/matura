#pragma once
#include <vector>
#include <string>
#include "Vector2D.h"
class Collider {
	
public:
	Collider() {
		height = 32;
		width = 32;
	}

	Collider(int xPos, int yPos) {
		x = xPos;
		y = yPos;
		height = 32;
		width = 32;
	}

	Collider(float xPos, float yPos, float _width, float _height) {
		x = xPos;
		y = yPos;
		height = _height;
		width = _width;
	}

	struct Point {
		float x;
		float y;
		float height;
		float width;
	};

	void FillColliders();
	std::string GetCollision(Vector2D Position,float width, float height, double angle, float dx, float dy);
	std::string GetCollisionTanks(Vector2D Position, float width, float height, double angle, float dx, float dy);
	float GetSpeed(Vector2D Position, int width, int height);
	int AddLaps(Vector2D Position, int width, int height, int laps, bool hasCrossedFinishLine);
	void GetCheckpoints();
	bool CheckForBox(float x , float y, int i, int j);
	void Draw();
	

	std::vector<Collider> border;
	std::vector<Collider> border_tank;
	std::vector<Collider> cesta;
	std::vector<Collider> cilj;
	std::vector<Collider> checkpoint;
	std::vector<Collider> fullCheckpoint;
	std::string vrsta;
	int x, y;
	int height, width;
	
	
};