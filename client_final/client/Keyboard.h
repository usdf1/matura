#pragma once

#include "SDL.h"

class Keyboard {

public:
	static Keyboard* GetInstance() {
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new Keyboard();
	}

	void Listen();
	bool GetKeyDown(SDL_Scancode key);

private:
	Keyboard();

	void KeyUp();
	void KeyDown();

	const Uint8* m_KeyStates;
	static Keyboard* s_Instance;
};