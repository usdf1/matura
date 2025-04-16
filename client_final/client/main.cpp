#include "Game.h"
#include "SDL.h"
#include <ctime>
#include <cstdlib>


int main(int argc, char* argv[]) {
	srand(time(NULL));
	Game::GetInstance()->Init();

	while ((Game::GetInstance()->IsRunning() == 1)) {
		Game::GetInstance()->Events();
		Game::GetInstance()->Update();
		Game::GetInstance()->Render();
	}

	Game::GetInstance()->Clean();

	return 0;
}