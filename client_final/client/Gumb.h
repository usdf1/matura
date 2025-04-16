#pragma once
#include "TextureManager.h"
#include <iostream>
#include "Game.h"
#include "Animation.h"
#include "Vector2D.h"
#include <vector>

class Gumb {

public:

	Gumb();
	Gumb(int x, int y, int width, int height, std::string id);
	void IsOver();
	void Draw();
	Vector2D GetPos() { return Pos; }
	void FillCarVector();
	void FillTankVector();
	void DrawAvtoSkini();
	void DrawTankSkini();

	bool hoovered;
	Vector2D Pos;
	float width, height;
	std::string id;

	std::vector <Gumb> AvtoSkin;
	std::vector <Gumb> TankSkin;

private:
	
	int mouseX, mouseY;
	float scale;
	Animation* m_Animation;


};