#include "AsteroidSystem.h"

AsteroidSystem::AsteroidSystem(EntityManager* eManager, SDL_Renderer* renderer) :
	renderer(renderer), eManager(eManager) {
	MessageManager::getInstance().subscribe<AsteroidAsteroidCollisionMessage>(
		[this](std::shared_ptr<AsteroidAsteroidCollisionMessage> msg) { handleAsteroidAsteroidCollision(msg); }
	);
	MessageManager::getInstance().subscribe<DestroyAsteroidMessage>(
		[this](std::shared_ptr<DestroyAsteroidMessage> msg) { handleDestroyAsteroidMessage(msg); }
	);
}

void AsteroidSystem::update(EntityManager* eManager, double timePassed) {
    //delete far asteroids
    generateAsteroids(eManager, timePassed);
}

void AsteroidSystem::generateAsteroids(EntityManager* eManager, double timePassed) {
    // insert logic with timing
    srand(time(NULL));
    int lvl = 1;
    generateSingleAsteroid(eManager, lvl);
}

void AsteroidSystem::generateSingleAsteroid(EntityManager* eManager, int lvl) {
    // std::cout << "Generating Asteroids: ";
	for (int i = asteroids.size(); i < MAX_ASTEROIDS; ++i) {
		std::cout << i << " ";
		uint32_t asteroid = eManager->createEntity();
		std::cout <<" with eID: " << asteroid << std::endl;
		eManager->addComponent(asteroid, ComponentType::Render);
		eManager->addComponent(asteroid, ComponentType::Physics);
		eManager->addComponent(asteroid, ComponentType::Collision);
		eManager->addComponent(asteroid, ComponentType::Type);
		eManager->addComponent(asteroid, ComponentType::Health);
		eManager->addComponent(asteroid, ComponentType::Animation);
		eManager->addComponent(asteroid, ComponentType::Damage);
		// Asteroid Texture
		RenderComponent astTexture(renderer, g_asteroid_big_surface);
		astTexture.texture->colorMod(Colors::Asteroid);
		eManager->setComponentData<RenderComponent>(asteroid, astTexture);
		// Asteroid Transform
		TransformComponent astTransform;
		astTransform.position = generatePosition(eManager);
		astTransform.rotDegrees = static_cast<double>(rand()) / RAND_MAX * 360 - 180; //Angle between 180 and -180
		eManager->setComponentData<TransformComponent>(asteroid, astTransform);
		// Asteroid Physics
		PhysicsComponent astPhys;
		astPhys.velocity = rand() % 100 + 50;
		eManager->setComponentData<PhysicsComponent>(asteroid, astPhys);
		// Asteroid Collider
		CollisionComponent astCollision;
		astCollision.shape = Shape::Circle;
		astCollision.radius = astTexture.texture->getWidth() / 2.0f;
		eManager->setComponentData<CollisionComponent>(asteroid, astCollision);
		// Asteroid type
		TypeComponent type = EntityType::Asteroid;
		eManager->setComponentData<TypeComponent>(asteroid, type);
		//Health
		HealthComponent health;
		health.health = 100.0f;
		health.maxHealth = 100.0f;
		eManager->setComponentData<HealthComponent>(asteroid, health);
		//Animation
		AnimationComponent anim;
		asteroids.emplace(asteroid);
		eManager->setComponentData<AnimationComponent>(asteroid, anim);
		//Damage to Player
		DamageComponent damageC;
		damageC.damage = 10 * lvl;
		eManager->setComponentData(asteroid, damageC);
	}
}

FPair AsteroidSystem::generatePosition(EntityManager* eManager) {
	const int maxAttempts = 50;
	int attempts = 0;
    // Missing logic if there are more than one player
    // srand(time(NULL));
    for (uint32_t playerID : eManager->getEntitiesWithComponent(ComponentType::Player)) {
        const TransformComponent* playerTransform = eManager->getComponentDataPtr<TransformComponent>(playerID);
        const PhysicsComponent* playerPhys = eManager->getComponentDataPtr<PhysicsComponent>(playerID);

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

			float minRadius = std::max(SCREEN_WIDTH, SCREEN_HEIGHT) * 0.75f;
			float maxRadius = minRadius * OUTER_RADIUS;
			float distance = minRadius + static_cast<float>(rand()) / RAND_MAX * (maxRadius - minRadius);
			float x = playerTransform->position.x + distance * sin(angle);
			float y = playerTransform->position.y - distance * cos(angle);
			SDL_FRect newAsteroidRect = { x, y, g_asteroid_big_surface.getWidth(), g_asteroid_big_surface.getHeight() };
			bool collisionFound = false;
			for (uint32_t existingAst : asteroids) {
				TransformComponent* existTr = eManager->getComponentDataPtr<TransformComponent>(existingAst);
				SDL_FRect existingRect = { 
					existTr->position.x,
					existTr->position.y,
					g_asteroid_big_surface.getWidth(),
					g_asteroid_big_surface.getHeight()
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
	PhysicsComponent pA = eManager->getComponentData<PhysicsComponent>(a);
	PhysicsComponent pB = eManager->getComponentData<PhysicsComponent>(b);
	TransformComponent tA = eManager->getComponentData<TransformComponent>(a);
	TransformComponent tB = eManager->getComponentData<TransformComponent>(b);
	CollisionComponent cA = eManager->getComponentData<CollisionComponent>(a);
	CollisionComponent cB = eManager->getComponentData<CollisionComponent>(b);
	// std::cout << "EntityA: " << a << " " << tA.position << std::endl;
	// std::cout << "EntityB: " << b << " " << tB.position << std::endl;
	FPair centerA = calculateCenters(tA.position.x, tA.position.y, cA.width, cA.height);
	FPair centerB = calculateCenters(tB.position.x, tB.position.y, cB.width, cB.height);

	FPair normal = centerB - centerA;
	normal.normalize();
	FPair relativeVel(pB.velocity * normal.x - pA.velocity * normal.x,
		pB.velocity * normal.y - pA.velocity * normal.y);
	// Calculate impulse (knockback strength)
	const float restitution = 1.0f; // "Bounciness" factor (0-1)
	const float knockbackMultiplier = 0.5f; // Extra push force
	float impulse = (1.0f + restitution) * (relativeVel.x + relativeVel.y) * knockbackMultiplier;

	pA.velocity = -pA.velocity - impulse * normal.x;
	pB.velocity = -pB.velocity + impulse * normal.y;

	const float separationForce = 0.50f;
	tA.position.x -= normal.x * separationForce;
	tA.position.y -= normal.y * separationForce;
	tB.position.x += normal.x * separationForce;
	tB.position.y += normal.y * separationForce;
	// std::cout << "EntityA: " << a << " new position " << tA.position << std::endl;
	// std::cout << "EntityB: " << b << " new position " << tB.position << std::endl;
	
	eManager->setComponentData<PhysicsComponent>(a, pA);
	eManager->setComponentData<PhysicsComponent>(b, pB);
	eManager->setComponentData<TransformComponent>(a, tA);
	eManager->setComponentData<TransformComponent>(b, tB);
}

void AsteroidSystem::handleDestroyAsteroidMessage(std::shared_ptr<DestroyAsteroidMessage> msg) {
	TransformComponent* trComp = eManager->getComponentDataPtr<TransformComponent>(msg->id);
	FPair center = { trComp->position.x + g_asteroid_big_surface.getWidth() / 2,
		trComp->position.y + g_asteroid_big_surface.getHeight() / 2 };
	MessageManager::getInstance().sendMessage(std::make_shared<ExperienceSpawnMessage>(center, 1));
	asteroids.erase(msg->id);
	eManager->destroyEntityLater(msg->id);
}
