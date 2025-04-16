#pragma once

#include "Game.h"

class Vector2D {
public:
	float X, Y;
	Vector2D(float x = 0, float y = 0) : X(x), Y(y) {}

	Vector2D operator+(Vector2D& v2) {
		return Vector2D(X + v2.X, Y + v2.Y);
	}

	Vector2D operator-(Vector2D& v2) {
		return Vector2D(X - v2.X, Y - v2.Y);
	}

	Vector2D operator*(float scale) {
		return Vector2D(X * scale, Y * scale);
	}

	void Log(std::string mag = "") {
		std::cout << mag << X << " " << Y << std::endl;
	}
};