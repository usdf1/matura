#pragma once

#include "SDL.h"
#include <vector>
#include "TextureManager.h"

class Mag {

public:
	std::vector <int> magazine;

	Uint32 relode;
	Uint32 fireRate;
	Uint32 relodeTime = 3000;

	void FillMagazine() {
		if(magazine.size() != 3) {
			while(magazine.size() != 3) {
				magazine.push_back(1);
			}
		}
	}

	void AddBullet() {
		if (SDL_GetTicks() - relode >= relodeTime && magazine.size() < 3) {
			magazine.push_back(1);
			relode = SDL_GetTicks();
		}
	};

	void SubBullet() {
		if (magazine.size() >= 1) {
			magazine.pop_back();
		}
		
	}

	void SetRelodeTime(int time) {
		relodeTime = static_cast<Uint32>(time);
	}

	void Draw() {
		if(Game::GetInstance()->relode == 0)
		{
			for (int i = 0; i < magazine.size(); i++) {
				TextureManager::GetInstance()->Draw("bullet", 40 * i, 800, 32, 64);
			}
		}

		if (Game::GetInstance()->relode == 1)
		{
			for (int i = 0; i < magazine.size(); i++) {
				TextureManager::GetInstance()->Draw("bullet_fire", 40 * i, 800, 32, 64);
			}
		}
	}

	bool GetDelay() {
		if (SDL_GetTicks() - fireRate >= 300) {
			fireRate = SDL_GetTicks();
			return 1;
		}

		return 0;
	}

};
