#pragma once

#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

constexpr auto SCREEN_WIDTH = /*1080;*/1780;
constexpr auto SCREEN_HEIGHT =/* 660;*/1060;

class Game {
public:

	static Game* GetInstance() {
		return s_instance = (s_instance != nullptr) ? s_instance : new Game();
	}

	bool Init();
	bool Clean();

	void Quit();

	void Update();
	void Render();
	void Events();

	bool clicked = 0;
	bool clickedTanks = 0;
	bool clickedSpomin = 0;
	int m_IsRunning;
	int m_State;
	inline int IsRunning() { return m_IsRunning; }
	inline int GetState() { return m_State; }
	inline SDL_Renderer* GetRenderer() { return m_renderer; }
	float car_scale;
	float tank_scale;
	bool canMove = 0;
	std::string PlayerName = "";
	std::string PlayerPassword = "";
	std::string PlayerNameReal = "";
	std::string PlayerPasswordReal = "";
	std::string SecretPassword = "";
	bool InPassword = 0;
	bool InUsername = 0;
	bool Exists = 0;

	bool zeObstaja = 0;
	bool IsNotEmail = 0;

	bool shield = 0;
	bool relode = 0;

	bool TurnEnded = 0;
	bool SpominStarted = 0;
	bool YourTurn = 0;
	int SpominTocka = 0;
	int NasprotnikTocke = 0;
	bool Is_Hit = 0;

	int skinMenu = 0;

	//bool Prijavljen = 0;

private:
	Game() {}
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	static Game* s_instance;
};