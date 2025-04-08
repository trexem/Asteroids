#include "AsteroidSystem.h"

void AsteroidSystem::update(EntityManager* eManager, double timePassed) {
    //delete far asteroids
    updateAsteroidCount(eManager);
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
	for (int i = asteroidCount; i < MAX_ASTEROIDS; ++i) {
		std::cout << i << " ";
		uint32_t asteroid = eManager->createEntity();
		std::cout <<" with eID: " << asteroid << std::endl;
		eManager->addComponent(asteroid, ComponentType::Render);
		eManager->addComponent(asteroid, ComponentType::Physics);
		eManager->addComponent(asteroid, ComponentType::Collision);
		eManager->addComponent(asteroid, ComponentType::Type);
		eManager->addComponent(asteroid, ComponentType::Health);
		eManager->addComponent(asteroid, ComponentType::Animation);
		// Asteroid Texture
		RenderComponent astTexture(renderer, g_asteroid_big_surface);
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
		astCollision.height = astTexture.texture->getHeight();
		astCollision.width = astTexture.texture->getWidth();
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
		eManager->setComponentData<AnimationComponent>(asteroid, anim);
        asteroidCount++;
	}
}

FPair AsteroidSystem::generatePosition(EntityManager* eManager) {
    // Missing logic if there are more than one player
    // srand(time(NULL));
    for (uint32_t playerID : eManager->getEntitiesWithComponent(ComponentType::Player)) {
        const TransformComponent* playerTransform = eManager->getComponentDataPtr<TransformComponent>(playerID);
        const PhysicsComponent* playerPhys = eManager->getComponentDataPtr<PhysicsComponent>(playerID);

        double movementAngle = playerTransform->rotDegrees * PI / 180; //Bias to spawn asteroids only in this direction
        if (playerPhys->velocity < 0) { //if player is going backwards the spawn should be biased the other way
            movementAngle = -movementAngle;
        } else if (playerPhys->velocity == 0) { //If player is not moving we spawn it anywhere
            movementAngle = static_cast<float>(rand()) / RAND_MAX * 2 * PI;
        }

        float angleOffset = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * ANGLE_BIAS_STRENGTH;
        float angle = movementAngle + angleOffset;

        float minRadius = std::max(SCREEN_WIDTH, SCREEN_HEIGHT) * 0.75f;
        float maxRadius = minRadius * OUTER_RADIUS;
        float distance = minRadius + static_cast<float>(rand()) / RAND_MAX * (maxRadius - minRadius);
        float x = playerTransform->position.x + distance * cos(angle);
        float y = playerTransform->position.y + distance * sin(angle);
        return FPair(x, y);
    }
    return FPair(0, 0);
}

void AsteroidSystem::updateAsteroidCount(EntityManager* eManager) {
    asteroidCount = 0;
    for (uint32_t eID : eManager->getEntitiesWithComponent(ComponentType::Type)) {
        const TypeComponent* typeComp = eManager->getComponentDataPtr<TypeComponent>(eID);
        if (typeComp->type == EntityType::Asteroid) {
            asteroidCount++;
        }
    }
}
