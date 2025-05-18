#include "AsteroidSystem.h"
#include "GameSessionManager.h"
#include "TextureManager.h"
#include "GUI.h"

AsteroidSystem::AsteroidSystem(EntityManager& eManager, SDL_Renderer* renderer) :
	renderer(renderer), eManager(eManager) {
	MessageManager::instance().subscribe<AsteroidAsteroidCollisionMessage>(
		[this](std::shared_ptr<AsteroidAsteroidCollisionMessage> msg) { handleAsteroidAsteroidCollision(msg); }
	);
	MessageManager::instance().subscribe<DestroyAsteroidMessage>(
		[this](std::shared_ptr<DestroyAsteroidMessage> msg) { handleDestroyAsteroidMessage(msg); }
	);
}

void AsteroidSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
    	generateAsteroids(eManager, dT);
	}
}

void AsteroidSystem::generateAsteroids(EntityManager& eManager, double timePassed) {
    // insert logic with timing
    int lvl = 1;
	const Uint32& time = GameStateManager::instance().getGameTimeSeconds();
	if (time % 2 == 0 && previousT != time) {
		for (int i = 0; i < 20; i++) {
			generateSingleAsteroid(eManager, lvl);
		}
	}
	previousT = time;
}

void AsteroidSystem::generateSingleAsteroid(EntityManager& eManager, int lvl) {
	if (asteroids.size() >= MAX_ASTEROIDS) return;
    // std::cout << "Generating Asteroids: ";
	uint32_t asteroid = eManager.createEntity();
	// std::cout <<" with eID: " << asteroid << std::endl;
	eManager.addComponent(asteroid, ComponentType::Render);
	eManager.addComponent(asteroid, ComponentType::Physics);
	eManager.addComponent(asteroid, ComponentType::Collision);
	eManager.addComponent(asteroid, ComponentType::Type);
	eManager.addComponent(asteroid, ComponentType::Health);
	eManager.addComponent(asteroid, ComponentType::Animation);
	eManager.addComponent(asteroid, ComponentType::Damage);
	// Asteroid Texture
	RenderComponent astTexture;
	astTexture.texture = TextureManager::instance().get("asteroid001");
	//astTexture.texture->colorMod(Colors::Asteroid);
	eManager.setComponentData<RenderComponent>(asteroid, astTexture);
	// Asteroid Transform
	TransformComponent astTransform;
	astTransform.position = generatePosition(eManager);
	uint32_t player = eManager.getEntitiesWithComponent(ComponentType::Player)[0];
	FPair playerPos = eManager.getComponentDataPtr<TransformComponent>(player)->position;
	astTransform.rotDegrees = static_cast<double>(rand()) / RAND_MAX * 45 
	+ astTransform.position.angleTowards(playerPos) * RAD2DEG; //Angle between 180 and -180
	eManager.setComponentData<TransformComponent>(asteroid, astTransform);
	// Asteroid Physics
	PhysicsComponent astPhys;
	astPhys.velocity = rand() % 100 + 50;
	eManager.setComponentData<PhysicsComponent>(asteroid, astPhys);
	// Asteroid Collider
	CollisionComponent astCollision;
	astCollision.shape = Shape::Circle;
	astCollision.radius = astTexture.texture->getWidth() / 2.0f;
	eManager.setComponentData<CollisionComponent>(asteroid, astCollision);
	// Asteroid type
	TypeComponent type = EntityType::Asteroid;
	eManager.setComponentData<TypeComponent>(asteroid, type);
	//Health
	HealthComponent health;
	health.health = ASTEROID_BASE_HEALTH * lvl;
	health.maxHealth = ASTEROID_BASE_HEALTH * lvl;
	eManager.setComponentData<HealthComponent>(asteroid, health);
	//Animation
	AnimationComponent anim;
	asteroids.emplace(asteroid);
	eManager.setComponentData<AnimationComponent>(asteroid, anim);
	//Damage to Player
	DamageComponent damageC;
	damageC.damage = 10 * lvl;
	eManager.setComponentData(asteroid, damageC);
}

FPair AsteroidSystem::generatePosition(EntityManager& eManager) {
	const int maxAttempts = 50;
	int attempts = 0;
    // Missing logic if there are more than one player
    // srand(time(NULL));
    for (uint32_t playerID : eManager.getEntitiesWithComponent(ComponentType::Player)) {
        const TransformComponent* playerTransform = eManager.getComponentDataPtr<TransformComponent>(playerID);
        const PhysicsComponent* playerPhys = eManager.getComponentDataPtr<PhysicsComponent>(playerID);

		while (attempts < maxAttempts) {
			double movementAngle = playerTransform->rotDegrees * DEG2RAD; //Bias to spawn asteroids only in this direction
			if (playerPhys->velocity < 0) { //if player is going backwards the spawn should be biased the other way
				movementAngle = -movementAngle;
			}
			else if (playerPhys->velocity == 0) { //If player is not moving we spawn it anywhere
				movementAngle = static_cast<float>(rand()) / RAND_MAX * TAU;
			}

			float angleOffset = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * ANGLE_BIAS_STRENGTH;
			float angle = movementAngle + angleOffset;

			float minRadius = std::max(GUI::screenWidth, GUI::screenHeight) * 0.75f;
			float maxRadius = minRadius * OUTER_RADIUS;
			float distance = minRadius + static_cast<float>(rand()) / RAND_MAX * (maxRadius - minRadius);
			float x = playerTransform->position.x + distance * sin(angle);
			float y = playerTransform->position.y - distance * cos(angle);
			SDL_FRect newAsteroidRect = { x, y, 100.0f, 100.0f };
			bool collisionFound = false;
			for (uint32_t existingAst : asteroids) {
				TransformComponent* existTr = eManager.getComponentDataPtr<TransformComponent>(existingAst);
				SDL_FRect existingRect = { 
					existTr->position.x,
					existTr->position.y,
					100.0f,
					100.0f
				};
				collisionFound = SDL_HasRectIntersectionFloat(&newAsteroidRect, &existingRect);
				if (collisionFound) break;
			}
			if (!collisionFound) return FPair(x, y);
			attempts++;
		}
    }
    return FPair(0, 0);
}

void AsteroidSystem::handleAsteroidAsteroidCollision(std::shared_ptr<AsteroidAsteroidCollisionMessage> msg) {
	const uint32_t a = msg->entityID[0];
    const uint32_t b = msg->entityID[1];
    
    // Get components by reference to avoid copies
    PhysicsComponent pA = eManager.getComponentData<PhysicsComponent>(a);
    PhysicsComponent pB = eManager.getComponentData<PhysicsComponent>(b);
    TransformComponent tA = eManager.getComponentData<TransformComponent>(a);
    TransformComponent tB = eManager.getComponentData<TransformComponent>(b);
    const auto& cA = eManager.getComponentData<CollisionComponent>(a);
    const auto& cB = eManager.getComponentData<CollisionComponent>(b);

    // Calculate centers (assuming position is center)
    FPair centerA = tA.position - cA.radius;
    FPair centerB = tB.position - cB.radius;
    
	// Collision normal (points from A to B)
	FPair normal = centerB - centerA;
	float distance = normal.length();
	normal.normalize();

	// Position correction first
	const float minSeparation = cA.radius + cB.radius;
	if (distance < minSeparation) {
		float correctionDepth = minSeparation - distance;
		FPair correction = normal * correctionDepth * 0.5f;
		tA.position -= correction;
		tB.position += correction;
		distance = minSeparation; // Update distance after correction
	}

	float radA = tA.rotDegrees * DEG2RAD;
	float radB = tB.rotDegrees * DEG2RAD;

	// Calculate direction vectors
	FPair dirA(cosf(radA), sinf(radA));
	dirA *= pA.velocity;
	FPair dirB(cosf(radB), sinf(radB));
	dirB *= pB.velocity;
	FPair vAn = dirA.project(normal);
	FPair vAt = dirA.reject(normal);
	
	FPair vBn = dirB.project(normal);
	FPair vBt = dirB.reject(normal);

	float uAn = vAn.dot(normal);
	float uBn = vBn.dot(normal);
	
	float combinedMass = pA.mass + pB.mass;
	float new_uAn = (uAn * (pA.mass - pB.mass) + 2 * pB.mass * uBn) / combinedMass;
	float new_uBn = (uBn * (pB.mass - pA.mass) + 2 * pA.mass * uAn) / combinedMass;

	FPair new_vAn = normal * new_uAn;
	FPair new_vBn = normal * new_uBn;

	dirA = new_vAn + vAt;
	dirB = new_vBn + vBt;
	tA.rotDegrees = atan2f(dirA.x, dirA.y) * RAD2DEG;
	tB.rotDegrees = atan2f(dirB.x, dirB.y) * RAD2DEG;
	pA.velocity = dirA.length();
	pB.velocity = dirB.length();

   	// Update components
	eManager.setComponentData(a, pA);
	eManager.setComponentData(b, pB);
	eManager.setComponentData(a, tA);
	eManager.setComponentData(b, tB);
}

void AsteroidSystem::handleDestroyAsteroidMessage(std::shared_ptr<DestroyAsteroidMessage> msg) {
	TransformComponent* trComp = eManager.getComponentDataPtr<TransformComponent>(msg->id);
	if (msg->playerDestroyed) {
		GameSessionManager::instance().getStats().asteroidsDestroyed++;
		FPair center = { trComp->position.x + 100.0f / 2,
			trComp->position.y + 100.0f / 2 };
		int goldProbLevel = GameStatsManager::instance().getStats().upgrades[UpgradeType::GoldProb];
		float goldProb = upgradesValues[static_cast<size_t>(UpgradeType::GoldProb)][goldProbLevel];
		int randomChance = rand() % 100;
		MessageManager::instance().sendMessage(std::make_shared<PickupsSpawnMessage>(center, 1, 
			randomChance < goldProb ? EntityType::Gold : EntityType::Experience ));
	}
	asteroids.erase(msg->id);
	eManager.destroyEntityLater(msg->id);
}

void AsteroidSystem::restart(EntityManager& eManager) {
	asteroidCount = 0;
	asteroids.clear();
}
