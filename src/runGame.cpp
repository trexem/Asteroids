#include "runGame.h"
#include "game.hpp"

int runGame() {
	Game game;
	bool is_initialized = game.initialize("We Need More Asteroids - by trexem",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED
	);
	if (!is_initialized) {
		std::cout << "Failed to initialize" << '\n';
	}
	else {
		bool is_media_loaded = game.loadMedia();
		if (!is_media_loaded) {
			std::cout << "Failed to load media!" << '\n';
		}
		else {
			game.start();
			game.gameLoop();//main loop
		}
	}

	return 0;
}
