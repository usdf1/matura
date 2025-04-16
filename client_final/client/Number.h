#pragma once

#include "TextureManager.h"

class Number {
		
public:
	int x, y; 
	float height, width;

	void Draw(Uint32 time) {
		
		if (time <= 5000 && time > 4000) {
			TextureManager::GetInstance()->DrawFrames("semafor", x, y, width, height, 2, 0, 1);
		}

		else if (time <= 4000 && time > 3000) {
			TextureManager::GetInstance()->DrawFrames("semafor", x, y, width, height, 0, 0, 1);
		}

		else if (time <= 3000 && time > 2000) {
			TextureManager::GetInstance()->DrawFrames("semafor", x, y, width, height, 0, 0, 1);
		}

		else if (time <= 2000 && time > 1000) {
			TextureManager::GetInstance()->DrawFrames("semafor", x, y, width, height, 1, 0, 1);
		}

		else if (time <= 1000 && time > 0) {
			TextureManager::GetInstance()->DrawFrames("semafor", x, y, width, height,1,0, 1);
		}
	
	}

	Number(int xP, int yP, float w, float h) {
		x = xP;
		y = yP;
		width = w;
		height = h;
	
	}


};

