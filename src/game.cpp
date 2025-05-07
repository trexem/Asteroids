#include "game.hpp"
#include "GameSessionManager.h"
#include "Systems.h"

#include <iostream>

Game::Game() : entityManager(MAX_ENTITIES) {
	quit = false;
	pause = false;
	last_tick = 0;
	tick = 0;
	GameStatsManager::instance().load("data/stats.json");
	inputSystem = std::make_unique<InputSystem>();
	playerSystem = std::make_unique<PlayerSystem>(&entityManager);
	physicsSystem = std::make_unique<PhysicsSystem>();
	movementSystem = std::make_unique<MovementSystem>(&camera);
	collisionSystem = std::make_unique<CollisionSystem>();
	damageSystem = std::make_unique<DamageSystem>(&entityManager);
	animationSystem = std::make_unique<AnimationSystem>(&entityManager);
	lifeTimeSystem = std::make_unique<LifeTimeSystem>(&entityManager);
	orbitSystem = std::make_unique<OrbitSystem>(&entityManager);
	followSystem = std::make_unique<FollowSystem>();
	healthSystem = std::make_unique<HealthSystem>();
	guiInteractionSystem = std::make_unique<GUIInteractionSystem>(&entityManager);
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
	
	screenManager.reset();
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
				screenManager = std::make_unique<ScreenManager>(&entityManager, renderSystem->getRenderer());
				pickupsSystem = std::make_unique<PickupsSystem>(&entityManager, renderSystem->getRenderer());
				abilitySystem = std::make_unique<AbilitySystem>(&entityManager, renderSystem->getRenderer());
				bgSystem = std::make_unique<BackgroundSystem>(entityManager, renderSystem->getRenderer());
				g_shot_texture.m_renderer = renderSystem->getRenderer();
				g_rocket_texture.m_renderer = renderSystem->getRenderer();
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
	asteroidSystem = std::make_unique<AsteroidSystem>(&entityManager, renderSystem->getRenderer());
	asteroidSystem->generateAsteroids(&entityManager, 0.0);
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
	asteroidSystem->restart(&entityManager);
	asteroidSystem->generateAsteroids(&entityManager, 0.0);
	GameStateManager::instance().setState(GameState::Playing);
}

void Game::gameLoop() {
	while(GameStateManager::instance().getState() != GameState::Quit) {
		auto loopTimeStart = std::chrono::high_resolution_clock::now();
		cap_timer.start(); //start cap timer at the beginning of the "frame"
		//Inputs
		inputSystem->update();
		if (GameStateManager::instance().getState() == GameState::Quit) break;
		//Calculate time between previous movement and now
		timeStep = step_timer.getTicks() / 1000.0;
		auto start = std::chrono::high_resolution_clock::now();
		auto end = std::chrono::high_resolution_clock::now();
		//UpdateSystems
		if (GameStateManager::instance().getState() == GameState::Playing) {
			start = std::chrono::high_resolution_clock::now();
			playerSystem->update(timeStep);
			end = std::chrono::high_resolution_clock::now();
			std::cout << "playerSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			start = std::chrono::high_resolution_clock::now();
			physicsSystem->update(&entityManager, timeStep);
			end = std::chrono::high_resolution_clock::now();
			std::cout << "physicsSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			start = std::chrono::high_resolution_clock::now();
			movementSystem->update(&entityManager, timeStep);
			end = std::chrono::high_resolution_clock::now();
			std::cout << "movementSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			start = std::chrono::high_resolution_clock::now();
			orbitSystem->update(timeStep);
			end = std::chrono::high_resolution_clock::now();
			std::cout << "orbitSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			start = std::chrono::high_resolution_clock::now();
			followSystem->update(&entityManager);
			end = std::chrono::high_resolution_clock::now();
			std::cout << "followSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			//Update LifeTime System
			start = std::chrono::high_resolution_clock::now();
			lifeTimeSystem->update(timeStep);
			end = std::chrono::high_resolution_clock::now();
			std::cout << "LifeTimeSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			//Check collisions
			start = std::chrono::high_resolution_clock::now();
			collisionSystem->update(&entityManager);
			end = std::chrono::high_resolution_clock::now();
			std::cout << "collisionSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			//update asteroids
			start = std::chrono::high_resolution_clock::now();
			asteroidSystem->update(&entityManager, timeStep);
			end = std::chrono::high_resolution_clock::now();
			std::cout << "asteroidSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			//Update damageSystem
			start = std::chrono::high_resolution_clock::now();
			damageSystem->update(timeStep);
			end = std::chrono::high_resolution_clock::now();
			std::cout << "damageSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			//Update abilities
			start = std::chrono::high_resolution_clock::now();
			abilitySystem->update();
			end = std::chrono::high_resolution_clock::now();
			std::cout << "abilitySystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			//Update Pickups
			start = std::chrono::high_resolution_clock::now();
			pickupsSystem->update();
			end = std::chrono::high_resolution_clock::now();
			std::cout << "pickupsSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
			healthSystem->update(&entityManager, timeStep);
			bgSystem->update(entityManager);
		}
		//animations
		start = std::chrono::high_resolution_clock::now();
		animationSystem->update(timeStep);
		end = std::chrono::high_resolution_clock::now();
		std::cout << "animationSystem time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
		if (GameStateManager::instance().getState() == GameState::Restart) {
			restart();
		}
		//GUI
		start = std::chrono::high_resolution_clock::now();
		screenManager->update();
		end = std::chrono::high_resolution_clock::now();
		std::cout << "ScreenManager time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
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
		//Wait time to cap FPS at 60
		int frame_ticks = cap_timer.getTicks();
		start = std::chrono::high_resolution_clock::now();
		if (frame_ticks < SCREEN_TICKS_PER_FRAME) {
			//Wait remaining time
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
		}
		end = std::chrono::high_resolution_clock::now();
		std::cout << "Waiting time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
		auto loopTimeEnd = std::chrono::high_resolution_clock::now();
		std::cout << "*#*# LOOP time: " << std::chrono::duration_cast<std::chrono::microseconds>(loopTimeEnd - loopTimeStart).count() << " us #*#*\n";
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
	shipTransform.position.x = SCREEN_WIDTH / 2;
	shipTransform.position.y = SCREEN_HEIGHT / 2;
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
