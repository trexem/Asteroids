#include "game.hpp"

#include <iostream>

Game::Game() : entityManager(MAX_ENTITIES) {
	quit = false;
	pause = false;
	last_tick = 0;
	tick = 0;
	inputSystem = std::make_unique<InputSystem>();
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
	std::cout << "Destroying Game...\n";

	g_ship_texture.free();
	g_particle_texture.free();
	g_particle_shimmer_texture.free();
	g_shot_texture.free();
	g_asteroid_big_texture.free();
	m_fps_text_texture.free();
	m_pause_text_texture.free();
	m_score_text_texture.free();
	// g_ship_surface.free();
	// g_shot_surface.free();
	// g_particle_surface;
 	// g_particle_shimmer_surface;
	// g_asteroid_big_surface;

	// MessageManager::getInstance().cleanup();
	
	guiSystem.reset();
    std::cout << "GUI System destroyed\n";
	playerSystem.reset();
	std::cout << "Player System destroyed\n";
	physicsSystem.reset();
	std::cout << "Physics System destroyed\n";
	movementSystem.reset();
	std::cout << "Movement System destroyed\n";
	collisionSystem.reset();
	std::cout << "Collision System destroyed\n";
	abilitySystem.reset();
	std::cout << "Ability System destroyed\n";
	damageSystem.reset();
	std::cout << "Damage System destroyed\n";
	animationSystem.reset();
	std::cout << "Animation System destroyed\n";
	asteroidSystem.reset();
	std::cout << "asteroidSystem destroyed\n";

	std::cout << "Before EntityManager clear\n";
	entityManager.clear();
	std::cout << "EntityManager cleared\n";

	std::cout << "Before render reset";
    renderSystem.reset();
    std::cout << "Render System destroyed\n";

	Fonts::cleanFonts();
	TTF_Quit();
	SDL_Quit();
}

bool Game::initialize(const char* t_title, int t_x, int t_y, int t_width, int t_height, Uint32 flags) {
	bool success = true;
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		std::cout << "SDL could not initialize! SDL_ERROR: " << SDL_GetError() << '\n';
		success = false;
	} else {
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
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
				guiSystem = std::make_unique<GUISystem>(&entityManager, renderSystem->getRenderer());
				xpSystem = std::make_unique<ExperienceSystem>(&entityManager, renderSystem->getRenderer());
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

	Fonts::loadFonts();
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
	counted_frames = 0;
	asteroidSystem = std::make_unique<AsteroidSystem>(renderSystem->getRenderer());
	asteroidSystem->generateAsteroids(&entityManager, 0.0);
	//start fps timer
	fps_timer.start();
	//Initialize srand with time so it-s always different
	srand(time(NULL));
	GameStateManager::getInstance().setState(GameState::MainMenu);
}

void Game::restart() {
	createShip(ShipType::TANK);
	asteroidSystem->generateAsteroids(&entityManager, 0.0);
	restartScore();
}

void Game::gameLoop() {
	while(GameStateManager::getInstance().getState() != GameState::Quit) {
		cap_timer.start(); //start cap timer at the beggining of the "frame"
		//Inputs
		inputSystem->update();
		if (GameStateManager::getInstance().getState() == GameState::Quit) break;
		//calculate fps: how many frames divided by the time that has passed since the game started
		float avg_fps = counted_frames / (fps_timer.getTicks() / 1000.f);
		if (avg_fps > 9999) {
			avg_fps = 0;
		}
		//Set text for the fps
		if (IS_FPS_VISIBLE) {
			time_text.str("");
			time_text << "Average FPS: " << avg_fps;
			if (!fpsTexture.texture->loadFromText(time_text.str().c_str(), Colors::White, Fonts::Body)) {
				std::cout << "Unable to render FPS texture!" << '\n';
			}
			// entityManager.setComponentData<RenderComponent>(fpsEntity, fpsTexture);
		}
		//Set Score text
		score_text.str("");
		score_text << "Score: " << m_score;
		if (!scoreTexture.texture->loadFromText(score_text.str().c_str(), Colors::White, Fonts::Body)) {
			std::cout << "Unable to render Score texture!" << '\n';
		}
		// entityManager.setComponentData<RenderComponent>(scoreEntity, scoreTexture);

		//Calculate time between previous movement and now
		time_step = step_timer.getTicks() / 1000.0;
		
		//UpdateSystems
		physicsSystem->update(&entityManager);
		playerSystem->update(time_step);
		movementSystem->update(&entityManager, time_step);
		//Check collisions
		collisionSystem->update(&entityManager);
		//animations
		animationSystem->update(time_step);
		//update asteroids
		asteroidSystem->update(&entityManager, time_step);
		//Update Experience
		xpSystem->update();
		//GUI
		guiSystem->update();
		if (GameStateManager::getInstance().getState() == GameState::Quit) break;
		//restart step 
		step_timer.start();
		//Render PAUSE text while game is paused
		if (GameStateManager::getInstance().getState() == GameState::Paused) {
			step_timer.pause();
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
	RenderComponent shipTexture = RenderComponent(renderSystem->getRenderer(), g_ship_surface);
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
