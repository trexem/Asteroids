#include "game.hpp"

#include <iostream>

Game::Game() : entityManager(MAX_ENTITIES) {
	quit = false;
	pause = false;
	last_tick = 0;
	tick = 0;
	keySystem = std::make_unique<KeyboardSystem>();
	uiSystem = std::make_unique<UISystem>();
	playerSystem = std::make_unique<PlayerSystem>(&entityManager);
	physicsSystem = std::make_unique<PhysicsSystem>();
	movementSystem = std::make_unique<MovementSystem>(&camera);
	collisionSystem = std::make_unique<CollisionSystem>();
	abilitySystem = std::make_unique<AbilitySystem>(&entityManager);
	damageSystem = std::make_unique<DamageSystem>(&entityManager);
	animationSystem = std::make_unique<AnimationSystem>(&entityManager);
	fpsTexture.texture = &m_fps_text_texture;
	scoreTexture.texture = &m_score_text_texture;
	pauseTexture.texture = &m_pause_text_texture;
}

Game::~Game() {
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
			renderSystem = std::make_unique<RenderSystem>(m_window.getWindow(), "", &camera);
			if (!renderSystem->getRenderer()) {
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				success = false;
			} else {
				m_fps_text_texture.m_renderer = renderSystem->getRenderer();
				m_pause_text_texture.m_renderer = renderSystem->getRenderer();
				m_score_text_texture.m_renderer = renderSystem->getRenderer();
				g_shot_texture.m_renderer = renderSystem->getRenderer();
				//init SDL_ttf
				if (!TTF_Init()) {
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
	if (!g_ship_surface.loadFromFile("data/img/spaceship.bmp")) {
		printf("Failed to load ship texture");
		success = false;
	}
	if (!g_shot_texture.loadFromFile("data/img/shot.png")) {
		printf("Failed to load shot texture");
		success = false;
	}
	if (!g_particle_surface.loadFromFile("data/img/ship_particle.bmp")) {
		printf("Failed to load ship particle texture");
		success = false;
	}
	if (!g_particle_shimmer_surface.loadFromFile("data/img/ship_particle_shimmer.bmp")) {
		printf("Failed to load ship shimmer particle texture");
		success = false;
	}
	if (!g_asteroid_big_surface.loadFromFile("data/img/asteroid1.bmp")) {
		printf("Failed to load asteroid big 1 texture");
		success = false;
	}

	return success;
}

void Game::start() {
	createShip(ShipType::TANK);
	pause_text.str("");
	pause_text << "PAUSE";
	counted_frames = 0;
	fpsEntity = entityManager.createEntity();
	TypeComponent uexType = EntityType::GUI;
	entityManager.addComponent(fpsEntity, ComponentType::Render);
	entityManager.addComponent(fpsEntity, ComponentType::Type);
	TransformComponent trComp;
	entityManager.setComponentData<RenderComponent>(fpsEntity, fpsTexture);
	trComp.position = FPair(0 , 0);
	entityManager.setComponentData<TransformComponent>(fpsEntity, trComp);
	entityManager.setComponentData<TypeComponent>(fpsEntity, uexType);
	scoreEntity = entityManager.createEntity();
	entityManager.addComponent(scoreEntity, ComponentType::Render);
	entityManager.addComponent(scoreEntity, ComponentType::Type);
	trComp.position = SCREEN_TOP_CENTER;
	entityManager.setComponentData<RenderComponent>(scoreEntity, scoreTexture);
	entityManager.setComponentData<TransformComponent>(scoreEntity, trComp);
	entityManager.setComponentData<TypeComponent>(scoreEntity, uexType);
	pauseEntity = entityManager.createEntity();
	entityManager.addComponent(pauseEntity, ComponentType::Render);
	entityManager.addComponent(pauseEntity, ComponentType::Type);
	trComp.position = FPair(-200, -200);
	entityManager.setComponentData<TransformComponent>(pauseEntity, trComp);
	entityManager.setComponentData<TypeComponent>(pauseEntity, uexType);
	std::cout << "fps eID: " << fpsEntity << std::endl;
	std::cout << "score eID: " << scoreEntity << std::endl;
	std::cout << "pause eID: " << pauseEntity << std::endl;
	if (!pauseTexture.texture->loadFromRenderedText(pause_text.str().c_str(), white_color, m_pause_ttf)) {
		std::cout << "Unable to render FPS texture!" << '\n';
	}
	entityManager.setComponentData<RenderComponent>(pauseEntity, pauseTexture);
	generateAsteroids();
	//start fps timer
	fps_timer.start();
	//Initialize srand with time so it-s always different
	srand(time(NULL));
}

void Game::restart() {
	generateAsteroids();
	restartScore();
}

void Game::gameLoop() {
	cap_timer.start(); //start cap timer at the beggining of the "frame"
	while (SDL_PollEvent(&e)) {
		keySystem->handleEvent(e);
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
	// const bool* current_key_states = SDL_GetKeyboardState(NULL);

	//calculate fps: how many frames divided by the time that has passed since the game started
	float avg_fps = counted_frames / (fps_timer.getTicks() / 1000.f);
	if (avg_fps > 9999) {
		avg_fps = 0;
	}
	//Set text for the fps
	if (IS_FPS_VISIBLE) {
		time_text.str("");
		time_text << "Average FPS: " << avg_fps;
		if (!fpsTexture.texture->loadFromRenderedText(time_text.str().c_str(), white_color, m_fps_ttf)) {
			std::cout << "Unable to render FPS texture!" << '\n';
		}
		entityManager.setComponentData<RenderComponent>(fpsEntity, fpsTexture);
	}
	//Set Score text
	score_text.str("");
	score_text << "Score: " << m_score;
	if (!scoreTexture.texture->loadFromRenderedText(score_text.str().c_str(), white_color, m_score_ttf)) {
		std::cout << "Unable to render Score texture!" << '\n';
	}
	entityManager.setComponentData<RenderComponent>(scoreEntity, scoreTexture);

	//Calculate time between previous movement and now
	time_step = step_timer.getTicks() / 1000.0;
	
	//UpdateSystems
	physicsSystem->update(&entityManager);
	playerSystem->update(time_step);
	movementSystem->update(&entityManager, time_step);
	//Check collisions
	collisionSystem->update(&entityManager);
	animationSystem->update(time_step);
	//restart step 
	step_timer.start();
	    //Render PAUSE text while game is paused
	if (pause) {
		step_timer.pause();
		TransformComponent tr;
		tr.position = SCREEN_CENTER;
		entityManager.setComponentData<TransformComponent>(pauseEntity, tr);
	} else {
		TransformComponent tr;
		tr.position = FPair(-200, -200);
		entityManager.setComponentData<TransformComponent>(pauseEntity, tr);
	}
	//display in window the render
	renderSystem->render(entityManager);
	++counted_frames;
	//Wait time to cap FPS at 60
	int frame_ticks = cap_timer.getTicks();
	if (frame_ticks < SCREEN_TICKS_PER_FRAME) {
		//Wait remaining time
		SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
	}
}

void Game::generateAsteroids() {
	std::cout << "Generating Asteroids: ";
	for (int i = 0; i < TOTAL_ASTEROIDS; ++i) {
		std::cout << i << " ";
		uint32_t asteroid = entityManager.createEntity();
		std::cout <<" with eID: " << asteroid << std::endl;
		entityManager.addComponent(asteroid, ComponentType::Render);
		entityManager.addComponent(asteroid, ComponentType::Physics);
		entityManager.addComponent(asteroid, ComponentType::Collision);
		entityManager.addComponent(asteroid, ComponentType::Type);
		entityManager.addComponent(asteroid, ComponentType::Health);
		entityManager.addComponent(asteroid, ComponentType::Animation);
		// Asteroid Texture
		RenderComponent astTexture(renderSystem->getRenderer(), g_asteroid_big_surface);
		entityManager.setComponentData<RenderComponent>(asteroid, astTexture);
		// Asteroid Transform
		FPair p = generateSingleAsteroidPos();
		TransformComponent astTransform;
		astTransform.position = p;
		astTransform.rotDegrees =  (atan((SCREEN_CENTER.x - p.x - astTexture.texture->getWidth() / 2) 
			/ (SCREEN_CENTER.y - p.y - astTexture.texture->getHeight() / 2)) - .087 + (rand() % 175) / 174)
			* 180 / PI;
		if (SCREEN_CENTER.y - p.y < 0) {
			astTransform.rotDegrees += PI;
		}
		entityManager.setComponentData<TransformComponent>(asteroid, astTransform);
		// Asteroid Physics
		PhysicsComponent astPhys;
		astPhys.velocity = rand() % 100 + 50;
		entityManager.setComponentData<PhysicsComponent>(asteroid, astPhys);
		// Asteroid Collider
		CollisionComponent astCollision;
		astCollision.height = astTexture.texture->getHeight();
		astCollision.width = astTexture.texture->getWidth();
		entityManager.setComponentData<CollisionComponent>(asteroid, astCollision);
		// Asteroid type
		TypeComponent type = EntityType::Asteroid;
		entityManager.setComponentData<TypeComponent>(asteroid, type);
		//Health
		HealthComponent health;
		health.health = 100.0f;
		health.maxHealth = 100.0f;
		entityManager.setComponentData<HealthComponent>(asteroid, health);
		//Animation
		AnimationComponent anim;
		entityManager.setComponentData<AnimationComponent>(asteroid, anim);
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

void Game::scoreUp() {
	m_score++;
}

void Game::restartScore() {
	m_score = 0;
}

void Game::createShip(ShipType shipType) {
	uint32_t ship = entityManager.createEntity();
	std::cout << "Ship eID: " << ship << std::endl;
	entityManager.addComponent(ship, ComponentType::Player);
	entityManager.addComponent(ship, ComponentType::Physics);
	entityManager.addComponent(ship, ComponentType::Stats);
	entityManager.addComponent(ship, ComponentType::Render);
	entityManager.addComponent(ship, ComponentType::Movement);
	entityManager.addComponent(ship, ComponentType::Collision);
	entityManager.addComponent(ship, ComponentType::Type);
	entityManager.addComponent(ship, ComponentType::Health);
	entityManager.addComponent(ship, ComponentType::Animation);
	 //Transform
	TransformComponent shipTransform;
	shipTransform.position.x = SCREEN_WIDTH / 2;
	shipTransform.position.y = SCREEN_HEIGHT / 2;
	entityManager.setComponentData<TransformComponent>(ship, shipTransform);
	//Render
	RenderComponent shipTexture =  RenderComponent(renderSystem->getRenderer(), g_ship_surface);
	entityManager.setComponentData<RenderComponent>(ship, shipTexture);
	// Stats
	StatsComponent shipStats;
	shipStats.maxSpeed = SHIP_TOP_SPEED;
	shipStats.minSpeed = SHIP_MIN_SPEED;
	shipStats.maxRotationSpeed = SHIP_TOP_ROTATION_SPEED;
	shipStats.speed = SHIP_SPEED;
	shipStats.rotationSpeed = SHIP_ROT_SPEED;
	shipStats.fireSpeed = SHIP_SHOT_DELAY;
	shipStats.collectionRadius = SHIP_BASE_RADIUS;
	entityManager.setComponentData<StatsComponent>(ship, shipStats);
	// Physics
	PhysicsComponent shipPhys;
	entityManager.setComponentData<PhysicsComponent>(ship, shipPhys);
	// Player
	PlayerComponent shipPlayer;
	shipPlayer.type = shipType;
	shipPlayer.abilities[static_cast<size_t>(ShipAbilities::LaserGun)] = true;
	shipPlayer.abilityLevels[static_cast<size_t>(ShipAbilities::LaserGun)] = 0;
	entityManager.setComponentData<PlayerComponent>(ship, shipPlayer);
	// Movement
	MovementComponent shipMovement;
	entityManager.setComponentData<MovementComponent>(ship, shipMovement);
	// Collision
	CollisionComponent shipCollider;
	shipCollider.height = shipTexture.texture->getHeight();
	shipCollider.width = shipTexture.texture->getWidth();
	entityManager.setComponentData<CollisionComponent>(ship, shipCollider);
	// Type
	TypeComponent type = EntityType::Player;
	entityManager.setComponentData<TypeComponent>(ship, type);
	//Health
	HealthComponent health;
	health.health = 100.0f;
	health.maxHealth = 100.0f;
	entityManager.setComponentData<HealthComponent>(ship, health);
	//Animation
	AnimationComponent anim;
	entityManager.setComponentData<AnimationComponent>(ship, anim);
}
