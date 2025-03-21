#include "game.hpp"

#include <iostream>

Game::Game() {
	quit = false;
	pause = false;
	last_tick = 0;
	tick = 0;
}

Game::~Game() {
	m_ship->free();
	m_fps_text_texture.free();
	m_pause_text_texture.free();
	g_ship_texture.free();
	g_particle_texture.free();
	g_particle_shimmer_texture.free();
	g_shot_texture.free();
	g_asteroid_big_texture.free();
	TTF_CloseFont(m_fps_ttf);
	TTF_CloseFont(m_pause_ttf);
	TTF_CloseFont(m_score_ttf);
	m_fps_ttf = nullptr;
	m_pause_ttf = nullptr;
	m_score_ttf = nullptr;
	TTF_Quit();
	SDL_Quit();
}

bool Game::initialize(const char* t_title, int t_x, int t_y, int t_width, int t_height, Uint32 flags) {
	bool success = true;
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "SDL could not initialize! SDL_ERROR: " << SDL_GetError() << '\n';
		success = false;
	} else {
		m_window = Window(t_title, t_x, t_y, t_width, t_height, flags);
		if (!m_window.getWindow()) {
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
			success = false;
		} else {
			renderSystem = new RenderSystem(m_window.getWindow(), NULL);
			if (!renderSystem->getRenderer()) {
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				success = false;
			} else {
				m_fps_text_texture.m_renderer = renderSystem->getRenderer();
				m_pause_text_texture.m_renderer = renderSystem->getRenderer();
				m_score_text_texture.m_renderer = renderSystem->getRenderer();
				g_ship_texture.m_renderer = renderSystem->getRenderer();
				g_shot_texture.m_renderer = renderSystem->getRenderer();
				g_particle_texture.m_renderer = renderSystem->getRenderer();
				g_particle_shimmer_texture.m_renderer = renderSystem->getRenderer();
				g_asteroid_big_texture.m_renderer = renderSystem->getRenderer();
				    //init SDL_ttf
				if (!TTF_Init())
				{
					std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << SDL_GetError() << '\n';
					success = false;
				}
			}
		}
	}
	return success;
}

bool Game::loadMedia() {
	bool success = true;

	m_fps_ttf = TTF_OpenFont("data/fonts/consola.ttf", 14);
	if (m_fps_ttf == NULL) {
		std::cout << "Failed to load consola font! SDL_ttf Error:" << SDL_GetError() << '\n';
		success = false;
	}
	m_pause_ttf = TTF_OpenFont("data/fonts/consola.ttf", 58);
	if (m_pause_ttf == NULL) {
		std::cout << "Failed to load consola font! SDL_ttf Error:" << SDL_GetError() << '\n';
		success = false;
	}
	m_score_ttf = TTF_OpenFont("data/fonts/consola.ttf", 18);
	if (m_score_ttf == NULL) {
		std::cout << "Failed to load consola font! SDL_ttf Error:" << SDL_GetError() << '\n';
		success = false;
	}
	if (!g_ship_texture.loadFromFile("data/img/spaceship.bmp")) {
		printf("Failed to load ship texture");
		success = false;
	}
	if (!g_shot_texture.loadFromFile("data/img/shot.png")) {
		printf("Failed to load shot texture");
		success = false;
	}
	if (!g_particle_texture.loadFromFile("data/img/ship_particle.bmp")) {
		printf("Failed to load ship particle texture");
		success = false;
	}
	if (!g_particle_shimmer_texture.loadFromFile("data/img/ship_particle_shimmer.bmp")) {
		printf("Failed to load ship shimmer particle texture");
		success = false;
	}
	if (!g_asteroid_big_texture.loadFromFile("data/img/asteroid1.bmp")) {
		printf("Failed to load asteroid big 1 texture");
		success = false;
	}

	return success;
}

void Game::start() {
	uint32_t ship = entityManager.createEntity();
	entityManager.addComponent(ship, ComponentType::Player);
	entityManager.addComponent(ship, ComponentType::Physics);
	entityManager.addComponent(ship, ComponentType::Render);
	TransformComponent shipTransform;
	shipTransform.Position.x = SCREEN_WIDTH / 2;
	shipTransform.Position.y = SCREEN_HEIGHT / 2;
	entityManager.setComponentData<TransformComponent>(ship, shipTransform);
	RenderComponent shipTexture;
	shipTexture.texture = &g_ship_texture;
	entityManager.setComponentData<RenderComponent>(ship, shipTexture);
	pause_text.str("");
	pause_text << "PAUSE";
	counted_frames = 0;
	if (!m_pause_text_texture.loadFromRenderedText(pause_text.str().c_str(), white_color, m_pause_ttf)) {
		std::cout << "Unable to render FPS texture!" << '\n';
	}
	generateAsteroids();
	//start fps timer
	fps_timer.start();
	//Initialize srand with time so it-s always different
	srand(time(NULL));
	uiSystem = UISystem();
	pSystem = PlayerSystem();
}

void Game::restart() {
	m_ship->restart();
	deleteAsteroids();
	generateAsteroids();
	restartScore();
}

void Game::gameLoop() {
	cap_timer.start(); //start cap timer at the beggining of the "frame"
	while (SDL_PollEvent(&e) != 0) {
		keySystem.handleEvent(e);
		    //Quit if the X button is pressed
		if (e.type == SDL_EVENT_QUIT) {
			quit = true;
		} else if (e.type == SDL_EVENT_KEY_DOWN) {
			if (e.key.key == SDLK_P) {//P for pause
				pause = !pause;
			}
			if (e.key.key == SDLK_R) {//R for restart. Will change in the future for an option in the pause menu
				restart();
			}
		}
	}
	    //The array current_key_states has the state of the pressed keys
	const bool* current_key_states = SDL_GetKeyboardState(NULL);

	    //calculate fps: how many frames divided by the time that has passed since the game started
	float avg_fps = counted_frames / (fps_timer.getTicks() / 1000.f);
	if (avg_fps > 9999) {
		avg_fps = 0;
	}
	    //Set text for the fps
	if (IS_FPS_VISIBLE) {
		time_text.str("");
		time_text << "Average FPS: " << avg_fps;
		if (!m_fps_text_texture.loadFromRenderedText(time_text.str().c_str(), white_color, m_fps_ttf)) {
			std::cout << "Unable to render FPS texture!" << '\n';
		}
	}
	//Set Score text
	score_text.str("");
	score_text << "Score: " << m_score;
	if (!m_score_text_texture.loadFromRenderedText(score_text.str().c_str(), white_color, m_score_ttf)) {
		std::cout << "Unable to render Score texture!" << '\n';
	}
	    //Calculate time between previous movement and now
	time_step = step_timer.getTicks() / 1000.0;
	    //while in pause, we don't take account of keys to move the spaceship
	if (!pause) {
		//m_ship->resume();
		//m_ship->handleInput(current_key_states);
	}
	    //move spaceship
	//m_ship->move(time_step);
	for (int i = 0; i < TOTAL_ASTEROIDS; ++i) {
		//m_asteroids[i]->move(time_step);
	}
	//Check collisions
	//checkCollisions();
	    //restart step timer
	step_timer.start();
	
	    //Clear renderer
	    //Render
	for (int i = 0; i < TOTAL_ASTEROIDS; ++i) {
		//m_asteroids[i]->render();
	}
	if (IS_FPS_VISIBLE) { m_fps_text_texture.render(0, 0); }
	m_score_text_texture.render(0, 0);
	    //Render PAUSE text while game is paused
	if (pause) {
		step_timer.pause();
		//m_ship->pause();
		m_pause_text_texture.render(
			SCREEN_WIDTH / 2 - m_pause_text_texture.getWidth() / 2,
			SCREEN_HEIGHT / 2 - m_pause_text_texture.getHeight() / 2
			);
	}
	    //display in window the render
		renderSystem->render(entityManager);
	++counted_frames;
	    //Wait time to cap FPS at 60
	int frame_ticks = cap_timer.getTicks();
	if (frame_ticks < SCREEN_TICKS_PER_FRAME)
	{
		    //Wait remaining time
		SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
	}
}

void Game::generateAsteroids() {
	std::cout << "Generating Asteroids: ";
	for (int i = 0; i < TOTAL_ASTEROIDS; ++i) {
		std::cout << i << " ";
		uint32_t asteroid = entityManager.createEntity();
		entityManager.addComponent(asteroid, ComponentType::Render);
		FPair p = generateSingleAsteroidPos();
		TransformComponent astTransform;
		astTransform.Position = p;
		RenderComponent astTexture;
		astTexture.texture = &g_asteroid_big_texture;
		entityManager.setComponentData<TransformComponent>(asteroid, astTransform);
		entityManager.setComponentData<RenderComponent>(asteroid, astTexture);
	}
	std::cout << std::endl;
}

FPair Game::generateSingleAsteroidPos() {
	int x_x = rand() % 2;
	int y_y = rand() % 2;
	float x = x_x > 0
		? rand() % 2160 - 120
		: (rand() % 240 - 360) + y_y * (SCREEN_WIDTH + 360);

	float y = x_x > 0
		? (rand() % 240 - 360) + y_y * (SCREEN_HEIGHT + 360)
		: rand() % 1320 - 120;
	
	return FPair(x, y);
}

void Game::checkCollisions() {
	// for (int i = 0; i < TOTAL_ASTEROIDS; ++i) {
	// 	//Check collision between ship and asteroid
	// 	if (checkCollision(m_ship->getCollider(), m_asteroids[i]->getCollider())) {
	// 		//TODO: Stop game, show final score and options menu
	// 		//for now let's restart
	// 		restart();
	// 	}
	// 	//check collisions between each shot and each asteroid
	// 	for (int j = 0; j < SHIP_MAX_SHOTS; ++j) {
	// 		if (m_ship->m_shots[j] != nullptr) {
	// 			if (checkCollision(m_ship->m_shots[j]->getCollider(), m_asteroids[i]->getCollider())) {
	// 				//Destroy asteroid
	// 				m_asteroids[i]->destroy();
	// 				//Destroy shot
	// 				m_ship->m_shots[j]->kill();
	// 				//+1 to score
	// 				scoreUp();
	// 			}
	// 		}
	// 	}
	// 	//check if asteroid is out of bounds
	// 	if ( m_asteroids[i]->getX() > 2300 || m_asteroids[i]->getX() < -500 ||
	// 		 m_asteroids[i]->getY() > 1500 || m_asteroids[i]->getY() < -500 ) {
	// 		//Destroy asteroid
	// 		m_asteroids[i]->destroy();
	// 	}

	// }
	// deleteDeadAsteroids();
}

void Game::deleteDeadAsteroids() {
	// for (int i = 0; i < TOTAL_ASTEROIDS; ++i) {
	// 	if (m_asteroids[i]->isDead()) {
	// 		delete m_asteroids[i];
			
	// 		FPair p = generateSingleAsteroidPos();
	// 		m_asteroids[i] = new Asteroid(p, BIG_ASTEROID);
	// 	}
	// }
}

void Game::deleteAsteroids() {
	// for (int i = 0; i < TOTAL_ASTEROIDS; ++i) {
	// 	delete m_asteroids[i];
	// }
}

void Game::scoreUp() {
	m_score++;
}

void Game::restartScore() {
	m_score = 0;
}