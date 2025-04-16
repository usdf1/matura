#include "Keyboard.h"
#include "Game.h"

Keyboard* Keyboard::s_Instance = NULL;

Keyboard::Keyboard() {
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Keyboard::Listen()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT: Game::GetInstance()->Quit();
			break;
		case SDL_KEYDOWN: KeyDown();
			break;
		case SDL_KEYUP: KeyUp();
			break;
		default:
			break;
		}
	}
}

bool Keyboard::GetKeyDown(SDL_Scancode key)
{
	return (m_KeyStates[key] == 1);
}

void Keyboard::KeyUp()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Keyboard::KeyDown()
{
	m_KeyStates = SDL_GetKeyboardState(NULL);
}