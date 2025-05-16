#include "game.hpp"
#include "GameSessionManager.h"
#include "Systems.h"
#include "SystemManager.h"
#include "MessageManager.h"
#include "GraphicsSettingsMessage.h"
#include "SettingsManager.h"

#include <iostream>
#include <SDL3/SDL_init.h>
#include <SDL3_mixer/SDL_mixer.h>

Game::Game() : entityManager(MAX_ENTITIES) {
	quit = false;
	pause = false;
	last_tick = 0;
	tick = 0;
	systemManager = std::make_unique<SystemManager>();
	GameStatsManager::instance().load("data/stats.json");
	systemManager->registerSystem<InputSystem>();
	systemManager->registerSystem<PlayerSystem>(entityManager);
	systemManager->registerSystem<PhysicsSystem>();
	systemManager->registerSystem<MovementSystem>(&camera);
	systemManager->registerSystem<CollisionSystem>();
	systemManager->registerSystem<DamageSystem>(entityManager);
	systemManager->registerSystem<AnimationSystem>(entityManager);
	systemManager->registerSystem<LifeTimeSystem>(entityManager);
	systemManager->registerSystem<OrbitSystem>(entityManager);
	systemManager->registerSystem<FollowSystem>();
	systemManager->registerSystem<HealthSystem>();
	systemManager->registerSystem<GUIInteractionSystem>(entityManager);

	MessageManager::instance().subscribe<GraphicsSettingsMessage>(
        [this](std::shared_ptr<GraphicsSettingsMessage> msg) { handleGraphicsSettingsMessage(msg); }
    );
}

Game::~Game() {
	std::cout << "Destroying Game...\n";

	g_ship_texture.free();
	g_particle_texture.free();
	g_particle_shimmer_texture.free();
	g_shot_texture.free();
	g_asteroid_big_texture.free();
	// g_ship_surface.free();
	// g_shot_surface.free();
	// g_particle_surface;
 	// g_particle_shimmer_surface;
	// g_asteroid_big_surface;

	// MessageManager::instance().cleanup();
	


	std::cout << "Before EntityManager clear\n";
	entityManager.clear();
	std::cout << "EntityManager cleared\n";

	systemManager->resetAll();
	Fonts::cleanFonts();
	TTF_Quit();
	SDL_Quit();
}

bool Game::initialize(const char* t_title, int t_x, int t_y) {
	bool success = true;
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO)) {
		std::cout << "SDL could not initialize! SDL_ERROR: " << SDL_GetError() << '\n';
		success = false;
	} else {
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
		GameSettings settings = SettingsManager::instance().get();
		Uint32 flags = SDL_WINDOW_BORDERLESS;
		if (settings.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
		m_window = Window(t_title, t_x, t_y, settings.screenWidth, settings.screenHeight, flags);
		if (settings.fullscreen) {
			SDL_SetWindowFullscreen(m_window.getWindow(), true);
			int w, h;
			SDL_GetWindowSize(m_window.getWindow(), &w, &h);
			std::cout << "Window Creation size: " << w << ", " << h << std::endl;
			SettingsManager::instance().setWindowSize(w, h);
		}
		if (!m_window.getWindow()) {
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
			success = false;
		} else {
			renderSystem = systemManager->registerSystem<RenderSystem>(m_window.getWindow(), "", &camera);
			if (!renderSystem->getRenderer()) {
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				success = false;
			} else {
				systemManager->registerSystem<ScreenManager>(entityManager, renderSystem->getRenderer());
				systemManager->registerSystem<PickupsSystem>(entityManager, renderSystem->getRenderer());
				systemManager->registerSystem<AbilitySystem>(entityManager, renderSystem->getRenderer());
				systemManager->registerSystem<BackgroundSystem>(entityManager, renderSystem->getRenderer());
				g_shot_texture.m_renderer = renderSystem->getRenderer();
				g_rocket_texture.m_renderer = renderSystem->getRenderer();
				//init SDL_ttf
				if (!TTF_Init()) {
					std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << SDL_GetError() << '\n';
					success = false;
				}
			}
		}
		systemManager->registerSystem<AudioSystem>();
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
	if (!g_rocket_texture.loadFromFile("data/img/rocket.bmp")) {
		printf("Failed to load rocket texture");
		success = false;
	}
	if (!g_particle_surface.loadFromFile("data/img/star1.bmp")) {
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
	createShip(ShipType::FREE_MOVE);
	counted_frames = 0;
	asteroidSystem = systemManager->registerSystem<AsteroidSystem>(entityManager, renderSystem->getRenderer());
	asteroidSystem->generateAsteroids(entityManager, 0.0);
	//start fps timer
	fps_timer.start();
	//Initialize srand with time so it-s always different
	srand(time(0));
	GameStateManager::instance().setState(GameState::MainMenu);
}

void Game::restart() {
	entityManager.clearGameEntities();
	GameSessionManager::instance().reset();
	counted_frames = 0;
	createShip(ShipType::FREE_MOVE);
	asteroidSystem->restart(entityManager);
	asteroidSystem->generateAsteroids(entityManager, 0.0);
	GameStateManager::instance().setState(GameState::Playing);
}

void Game::gameLoop() {
	while(GameStateManager::instance().getState() != GameState::Quit) {
		auto loopTimeStart = std::chrono::high_resolution_clock::now();
		cap_timer.start(); //start cap timer at the beginning of the "frame"
		//Inputs
		if (GameStateManager::instance().getState() == GameState::Quit) break;
		//Calculate time between previous movement and now
		timeStep = step_timer.getTicks() / 1000.0;
		systemManager->updateAll(entityManager, timeStep);
		auto start = std::chrono::high_resolution_clock::now();
		auto end = std::chrono::high_resolution_clock::now();
		if (GameStateManager::instance().getState() == GameState::Restart) {
			restart();
		}
		// std::cout << "ScreenManager time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
		if (GameStateManager::instance().getState() == GameState::Quit) break;
		//restart step 
		step_timer.start();
		//Render PAUSE text while game is paused
		if (GameStateManager::instance().getState() != GameState::Playing) {
			step_timer.pause();
		}
		//Destroy entities
		entityManager.applyPendindDestructions();
		//display in window the render
		renderSystem->render(entityManager);
		++counted_frames;

		if (shouldUpdateSettings) updateGraphicsSettings();
		if (pendingFullScreenChange) updateFullScreen();

		//Wait time to cap FPS at 60
		int frame_ticks = cap_timer.getTicks();
		start = std::chrono::high_resolution_clock::now();
		if (frame_ticks < SCREEN_TICKS_PER_FRAME) {
			//Wait remaining time
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
		}
		end = std::chrono::high_resolution_clock::now();
		// std::cout << "Waiting time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
		auto loopTimeEnd = std::chrono::high_resolution_clock::now();
		// std::cout << "*#*# LOOP time: " << std::chrono::duration_cast<std::chrono::microseconds>(loopTimeEnd - loopTimeStart).count() << " us #*#*\n";
	}
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
	shipTransform.position = GUI::screenCenter;
	//Render
	RenderComponent shipTexture = RenderComponent(renderSystem->getRenderer(), g_ship_surface);
	entityManager.setComponentData<RenderComponent>(ship, shipTexture);
	shipTransform.position.x -= g_ship_surface.getWidth() / 2;
	shipTransform.position.y -= g_ship_surface.getHeight() / 2;
	entityManager.setComponentData<TransformComponent>(ship, shipTransform);
	// Stats
	StatsComponent shipStats;
	shipStats.maxSpeed = SHIP_TOP_SPEED;
	shipStats.minSpeed = SHIP_MIN_SPEED;
	shipStats.maxRotationSpeed = SHIP_TOP_ROTATION_SPEED;
	shipStats.speed = SHIP_SPEED;
	shipStats.rotationSpeed = SHIP_ROT_SPEED;
	shipStats.fireSpeed = SHIP_SHOT_DELAY;
	// shipStats.collectionRadius = SHIP_BASE_RADIUS;
	shipStats.collectionRadius = 0;
	entityManager.setComponentData<StatsComponent>(ship, shipStats);
	// Physics
	PhysicsComponent shipPhys;
	shipPhys.isSpeedVector = true;
	entityManager.setComponentData<PhysicsComponent>(ship, shipPhys);
	// Player
	PlayerComponent shipPlayer;
	shipPlayer.type = shipType;
	// shipPlayer.ownedPassives[static_cast<size_t>(PassiveAbilities::PickupRadius)] = true;
	// shipPlayer.passiveLevels[static_cast<size_t>(PassiveAbilities::PickupRadius)] = 8;
	shipPlayer.ownedWeapons[static_cast<size_t>(WeaponAbilities::LaserGun)] = true;
	shipPlayer.weaponLevels[static_cast<size_t>(WeaponAbilities::LaserGun)] = 0;
	// shipPlayer.ownedWeapons[static_cast<size_t>(WeaponAbilities::GravitySaws)] = true;
	// shipPlayer.weaponLevels[static_cast<size_t>(WeaponAbilities::GravitySaws)] = 1;
	// shipPlayer.ownedWeapons[static_cast<size_t>(WeaponAbilities::Rocket)] = true;
	// shipPlayer.weaponLevels[static_cast<size_t>(WeaponAbilities::Rocket)] = 9;
	// shipPlayer.ownedWeapons[static_cast<size_t>(WeaponAbilities::Laser)] = true;
	// shipPlayer.weaponLevels[static_cast<size_t>(WeaponAbilities::Laser)] = 9;
	// shipPlayer.ownedWeapons[static_cast<size_t>(WeaponAbilities::Explosives)] = true;
	// shipPlayer.weaponLevels[static_cast<size_t>(WeaponAbilities::Explosives)] = 9;
	shipPlayer.ownedWeaponsCount = 1;
	// shipPlayer.currentXp = 100;
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
	health.health = SHIP_BASE_HEALTH;
	health.maxHealth = SHIP_BASE_HEALTH;
	entityManager.setComponentData<HealthComponent>(ship, health);
	//Animation
	AnimationComponent anim;
	entityManager.setComponentData<AnimationComponent>(ship, anim);
}

void Game::handleGraphicsSettingsMessage(std::shared_ptr<GraphicsSettingsMessage> msg) {
	shouldUpdateSettings = true;
	graphicsSettings = msg;
}

void Game::updateGraphicsSettings() {
	if (graphicsSettings->newSize != graphicsSettings->prevSize) {
		SDL_SetWindowSize(m_window.getWindow(), graphicsSettings->newSize.x, graphicsSettings->newSize.y);
	}
	SDL_SetWindowFullscreen(m_window.getWindow(), graphicsSettings->fullscreen);

	SDL_SetRenderVSync(renderSystem->getRenderer(), graphicsSettings->vsync ? 1 : SDL_RENDERER_VSYNC_DISABLED);
	
	pendingFullScreenChange = true;
	shouldUpdateSettings = false;
	graphicsSettings.reset();
}

void Game::updateFullScreen() {
	int w, h;
	SDL_GetRenderOutputSize(renderSystem->getRenderer(), &w, &h);
	std::cout << "REAL Window size after fullscreen toggle: " << w << " x " << h << std::endl;

	SettingsManager::instance().setWindowSize(w, h);
	pendingFullScreenChange = false;
}
