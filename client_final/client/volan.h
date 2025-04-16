#pragma once
#include "SDL.h"

class Volan {
private: 
	SDL_Rect Pos;
	Uint32 mouseButtons;
	int mouseX, mouseY;
	bool IsHeld;
	int volanCenterX;
	int volanCenterY;
	double initialMouseAngle;
	int lastMouseX, lastMouseY;

public:
	Volan() { Pos.x = 100; Pos.y = 100; Pos.h = 200; Pos.w = 200; IsHeld = 0; angle = 0.0; 
	int volanCenterX = Pos.x + Pos.w / 2;
	int volanCenterY = Pos.y + Pos.h / 2;
	initialMouseAngle = 0.0;
	lastMouseX = 0;
	lastMouseY = 0;
	}
	double CalculateAngle();
	void CheckForHold();
	bool isMouseInsideRect();
	bool ReturnIsHeld() { return IsHeld; }
	double angle;
	void Draw();
	void OnMouseDown();
	void OnMouseUp();

};