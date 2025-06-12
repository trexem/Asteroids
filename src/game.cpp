#include "game.hpp"
#include "GameSessionManager.h"
#include "Systems.h"
#include "SystemManager.h"
#include "MessageManager.h"
#include "GraphicsSettingsMessage.h"
#include "SettingsManager.h"
#include "TextureManager.h"
#include "PackReader.h"
#include "TextManager.h"
#include "SystemLocale.h"
#include "SpatialGridBuilder.h"

#include <iostream>
#include <SDL3/SDL_init.h>
#include <SDL3_mixer/SDL_mixer.h>

Game::Game() : entityManager(MAX_ENTITIES) {
	quit = false;
	pause = false;
	last_tick = 0;
	tick = 0;
	systemManager = std::make_unique<SystemManager>();
	GameStatsManager::instance().load();
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
	systemManager->registerSystem<TooltipSystem>();
	systemManager->registerSystem<ProximitySystem>();
	MessageManager::instance().subscribe<GraphicsSettingsMessage>(
        [this](std::shared_ptr<GraphicsSettingsMessage> msg) { handleGraphicsSettingsMessage(msg); }
    );

	if (!PackReader::instance().init("data.bin")) {
		DEBUG_LOG("Failed to load asset pack!");
	}
}

Game::~Game() {
	DEBUG_LOG("Destroying Game...");

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
	std::string userLang = SystemLocale::get();
	DEBUG_LOG("System language got is: %s", userLang.c_str());
	TextManager::instance().loadLanguage(userLang);
	// TextManager::instance().loadLanguage("es");
	bool success = true;
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO)) {
		std::cout << "SDL could not initialize! SDL_ERROR: " << SDL_GetError() << '\n';
		success = false;
	} else {
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
		GameSettings settings = SettingsManager::instance().get();
#if defined (__ANDROID__)
		settings.fullscreen = true;
#endif
		Uint32 flags = SDL_WINDOW_BORDERLESS;
		if (settings.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
		m_window = Window(t_title, t_x, t_y, settings.screenWidth, settings.screenHeight, flags);
		if (settings.fullscreen) {
			SDL_SetWindowFullscreen(m_window.getWindow(), true);
			int w, h, aux;
			SDL_GetWindowSize(m_window.getWindow(), &w, &h);
			aux = std::max(w, h);
			h = std::min(w, h);
			w = aux;
			DEBUG_LOG("Window Creation size: %d, %d", w, h);
			SettingsManager::instance().setWindowSize(w, h);
		}
		if (!m_window.getWindow()) {
			DEBUG_LOG("Window could not be created! SDL_Error: %s", SDL_GetError());
			success = false;
		} else {
			renderSystem = systemManager->registerSystem<RenderSystem>(m_window.getWindow(), "", &camera);
			if (!renderSystem->getRenderer()) {
				DEBUG_LOG("Renderer could not be created! SDL Error: %s", SDL_GetError());
				success = false;
			} else {
				//init SDL_ttf
				if (!TTF_Init()) {
					DEBUG_LOG("SDL_ttf could not initialize! SDL_ttf Error: %s", SDL_GetError());
					success = false;
				} else {
					DEBUG_LOG("Fonts Loaded Successfully");
				}
			}
		}
		systemManager->registerSystem<AudioSystem>();
	}
#if defined(__ANDROID__)
	SDL_Rect screenSize;
	SDL_GetDisplayBounds(1, &screenSize);
	DEBUG_LOG("Android screen size: %dx%d", screenSize.w, screenSize.h);
	SettingsManager::instance().setWindowSize(std::max(screenSize.w, screenSize.h), std::min(screenSize.w, screenSize.h));
#endif
	return success;
}

bool Game::loadMedia() {
	bool success = true;

	Fonts::loadFonts();
	TextureManager::instance().init(renderSystem->getRenderer());
	systemManager->registerSystem<ScreenManager>(entityManager, renderSystem->getRenderer());
	systemManager->registerSystem<PickupsSystem>(entityManager, renderSystem->getRenderer());
	systemManager->registerSystem<AbilitySystem>(entityManager, renderSystem->getRenderer());
	systemManager->registerSystem<BackgroundSystem>(entityManager, renderSystem->getRenderer());
#if defined (__ANDROID__)
	systemManager->registerSystem<TouchJoystickSystem>(entityManager);
#endif
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
        step_timer.start();

		SpatialGrid::instance().build(entityManager);

        systemManager->updateAll(entityManager, timeStep);
        if (GameStateManager::instance().getState() == GameState::Restart) {
            restart();
        }

        if (GameStateManager::instance().getState() == GameState::Quit) break;
        //restart step
        //Render PAUSE text while game is paused
        if (GameStateManager::instance().getState() != GameState::Playing) {
            step_timer.pause();
        }
        //Destroy entities
        entityManager.applyPendindDestructions();
        //display in window the render
        auto start = std::chrono::high_resolution_clock::now();
        renderSystem->render(entityManager);
        auto end = std::chrono::high_resolution_clock::now();
        // DEBUG_LOG("[RenderSystem] update time: %lld us", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        ++counted_frames;

		if (shouldUpdateSettings) updateGraphicsSettings();
		if (pendingFullScreenChange) updateFullScreen();

		//Wait time to cap FPS at 60
		Uint32 frame_ticks = cap_timer.getTicks();
		start = std::chrono::high_resolution_clock::now();
		if (frame_ticks < SCREEN_TICKS_PER_FRAME) {
			//Wait remaining time
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
		}
		end = std::chrono::high_resolution_clock::now();
		// DEBUG_LOG("Waiting time: %lld us", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		auto loopTimeEnd = std::chrono::high_resolution_clock::now();
		// DEBUG_LOG("*#*#  LOOP time: %lld us  *#*#", std::chrono::duration_cast<std::chrono::microseconds>(loopTimeEnd - loopTimeStart).count());
	}
}

void Game::createShip(ShipType shipType) {
	uint32_t ship = entityManager.createEntity();
	DEBUG_LOG("Ship eID: %d", ship);
	entityManager.addComponent(ship, ComponentType::Player);
	entityManager.addComponent(ship, ComponentType::Physics);
	entityManager.addComponent(ship, ComponentType::Stats);
	entityManager.addComponent(ship, ComponentType::Render);
	entityManager.addComponent(ship, ComponentType::Movement);
	entityManager.addComponent(ship, ComponentType::Collision);
	entityManager.addComponent(ship, ComponentType::Type);
	entityManager.addComponent(ship, ComponentType::Health);
	entityManager.addComponent(ship, ComponentType::Animation);
	entityManager.addComponent(ship, ComponentType::ProximityTracker);
	 //Transform
	TransformComponent shipTransform;
	shipTransform.position = GUI::screenCenter;
	//Render
	RenderComponent shipTexture;
	shipTexture.texture = TextureManager::instance().get("ship");
	shipTexture.exactSize = {50.0f, 50.0f};
	shipTexture.isStretched = true;
	entityManager.setComponentData<RenderComponent>(ship, shipTexture);
	shipTransform.position.x -= shipTexture.exactSize.x / 2;
	shipTransform.position.y -= shipTexture.exactSize.y / 2;
	entityManager.setComponentData<TransformComponent>(ship, shipTransform);
	// Stats
	StatsComponent shipStats;
	float speedMult = GameStatsManager::instance().getUpgradeValue(UpgradeType::Speed);
	shipStats.maxSpeed = SHIP_TOP_SPEED * speedMult;
	shipStats.minSpeed = SHIP_MIN_SPEED * speedMult;
	shipStats.maxRotationSpeed = SHIP_TOP_ROTATION_SPEED * speedMult;
	shipStats.speed = SHIP_SPEED * speedMult;
	shipStats.rotationSpeed = SHIP_ROT_SPEED  * speedMult;
	shipStats.fireSpeed = GameStatsManager::instance().getUpgradeValue(UpgradeType::FireRate);
	shipStats.baseDamage = GameStatsManager::instance().getUpgradeValue(UpgradeType::Damage);
	shipStats.armor = GameStatsManager::instance().getUpgradeValue(UpgradeType::Armor);
	shipStats.collectionRadius = GameStatsManager::instance().getUpgradeValue(UpgradeType::PickupRange);
	shipStats.projectileCount = GameStatsManager::instance().getUpgradeValue(UpgradeType::ProjectileCount);
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
	// shipPlayer.ownedWeapons[static_cast<size_t>(WeaponAbilities::TeslaGun)] = true;
	// shipPlayer.weaponLevels[static_cast<size_t>(WeaponAbilities::TeslaGun)] = 9;
	shipPlayer.ownedWeaponsCount = 1;
	// shipPlayer.currentXp = 100;
	entityManager.setComponentData<PlayerComponent>(ship, shipPlayer);
	// Movement
	MovementComponent shipMovement;
	entityManager.setComponentData<MovementComponent>(ship, shipMovement);
	// Collision
	CollisionComponent shipCollider;
	shipCollider.height = shipTexture.exactSize.x;
	shipCollider.width = shipTexture.exactSize.y;
	entityManager.setComponentData<CollisionComponent>(ship, shipCollider);
	// Type
	TypeComponent type = EntityType::Player;
	entityManager.setComponentData<TypeComponent>(ship, type);
	//Health
	HealthComponent health;
	float healthMult = GameStatsManager::instance().getUpgradeValue(UpgradeType::MaxHealth);
	health.health = SHIP_BASE_HEALTH * healthMult;
	health.maxHealth = SHIP_BASE_HEALTH * healthMult;
	health.regen = GameStatsManager::instance().getUpgradeValue(UpgradeType::HealthRegen);
	entityManager.setComponentData<HealthComponent>(ship, health);
	//Animation
	AnimationComponent anim;
	anim.animations[Animation::Damage].frames = TextureManager::instance().getAnimationFrames("ship", "Damage");
	entityManager.setComponentData<AnimationComponent>(ship, anim);
	// Proximity Tracker
	ProximityTrackerComponent prox;
	entityManager.setComponentData(ship, prox);
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
	DEBUG_LOG("REAL Window size after fullscreen toggle: %dx%d", w, h);
	std::cout <<  std::endl;

	SettingsManager::instance().setWindowSize(w, h);
	pendingFullScreenChange = false;
}
