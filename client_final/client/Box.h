#pragma once

#include "TextureManager.h"
#include <string>

class Box {
public:
	float x, y, width, height;
	int ID;
	int type;

	Box() {}

	Box(int _ID, float _x, float _y, int _type)
		: ID(_ID), x(_x), y(_y), type(_type)
	{
		width = 32;
		height = 32;
	}

	void Draw() { TextureManager::GetInstance()->Draw("box", x, y, width, height); }
	void Update();
};