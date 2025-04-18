# ASTEROIDS GAME by trexem
Mostly testing and playing with SDL3, it is still WIP so be patient, I'm learning about it. I'm trying to achieve an ECS-Message Architecture.

* [AsteroidsGame.cpp](https://github.com/trexem/Asteroids/blob/master/src/AsteroidsGame.cpp) is the main file
* [EntityManager.hpp](https://github.com/trexem/Asteroids/blob/master/include/EntityManager.h) holds the logic for Entities Creation.
* [texture.hpp](https://github.com/trexem/Asteroids/blob/master/include/texture.hpp) has the Texture class, which is used to load images and text into the screen.
* [timer.hpp](https://github.com/trexem/Asteroids/blob/master/include/timer.hpp) has a simple timer class, you can pause, resume and restart timer.
* [game.hpp](https://github.com/trexem/Asteroids/blob/master/include/game.hpp) holds the methods for the gameLoop, ship creation and asteroids generation. I still need to clean and refactor it.
* [components.h](https://github.com/trexem/Asteroids/blob/master/include/Components.h) holds the Components that each entity can have.
* [systems](https://github.com/trexem/Asteroids/tree/master/include/systems) holds the multiple systems the game will have. They are responsible for all the logic on how the entities and components should behave.


## Try out
### Linux
Clone the repo with
```shell
git clone https://github.com/trexem/Asteroids.git --depth=1 --recurse-submodules"
``` 
so it downloads the SDL submodules. After that you might still have to download some files for SDL to compile correctly, you might have to run `download.sh` inside `external` subdirectory of SDL_image and SDL_ttf. 
To generate make files you need to run
```shell
cmake -S . -B build
```
Then to compile we run 
```shell
cmake --build build
```
Alternatively to generate build files and build you can just run
```shell
./build.sh
```
Then just open the asteroids program.
```shell
./build/asteroids
```
### Windows
Open `Visual Studio 2022` and clone the repo through the initial options. It should download the submodules and everything necessary for the project to compile. 
After it finishes downloading and `cmake` stops working you will have to choose `asteroids.exe` solution (besides the "Play" button). Then just press the "Play" button.

## TODO
### Audio
 - [ ] Implement it
### Components
 - [X] TransformComponent 
 - [X] PhysicsComponent (dx, dy, speed)
 - [X] CollisionComponent (hitbox, isTrigger)
 - [X] RenderComponent (texture, size)
 - [X] HealthComponent (hp, maxHp)
 - [X] DamageComponent (damageAmount)
 - [X] PlayerComponent (input tracking)
 - [X] StatsComponent (fireRate, projectileType)
 - [X] MovementComponent (behaviorType: PLAYER, ENEMY, ASTEROID)
 - [X] TypeComponent (EntityType)
 - [X] ExperienceComponent (how much xp will give)
 - [X] AnimationComponent (render different textures for one entity)
 - [X] GUIComponent (for GUI/HUD components)
 - [ ] ExpirationComponent (for projectiles/lifespan)
 - [ ] ExperienceComponent (points on destruction)
 - [ ] AIComponent (for enemies with patterns)
 - [ ] SpawnerComponent (controls enemy/asteroid waves)
 ### Systems
 - [X] AbilitySystem (spawns bullets)
 - [X] AnimationSystem
 - [X] AsteroidSystem (might modify to SpawnSystem or handle that here)
 - [X] CollisionSystem (detects & sends collision messages)
 - [X] DamageSystem (resolves collision messages)
 - [X] ExperienceSystem (handles experience entities)
 - [X] GUISystem (handles screen creations and overlays)
 - [X] InputSystem (handles all inputs and sends iput messages)
 - [X] MovementSystem (updates position based on behavior)
 - [X] PlayerSystem (handles inputs and abilities for players)
 - [X] RenderSystem (renders all objects)
 - [ ] EnemyAISystem (moves enemies based on AIComponent)
 - [ ] SpawnSystem (handles waves of enemies & asteroids)
 - [ ] PowerUpSystem (applies power-ups)
 - [ ] ScoreSystem (tracks player score)
### Abilities
 - [X] LaserGun
 - [X] RocketLauncher
 - [ ] Laser
 - [ ] Explosives
 - [ ] GravitySaws
 - [X] PickupRadius
 - [ ] ideas on more abilities
