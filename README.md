# ASTEROIDS GAME by trexem
Mostly testing and playing with SDL2, it is still WIP so be patient, I'm learning about it.

You need to have installed SDL2 libraries, I'm using `g++` compiler in Ubuntu and `make` command to compile. But there're tutorials for you to install and compile SDL2 with MingW32, Codeblocks or whatever, also check [SDL official page](https://www.libsdl.org/)

* [AsteroidsGame.cpp](https://github.com/trexem/Asteroids/blob/master/Asteroids/src/AsteroidsGame.cpp) is the main file
* [Ship.hpp](https://github.com/trexem/Asteroids/blob/master/Asteroids/src/Ship.hpp) has the Shot and Ship classes
* [Texture.hpp](https://github.com/trexem/Asteroids/blob/master/Asteroids/src/Texture.hpp) has the Texture class, which is used to load images and text into the screen
* [Timer.hpp](https://github.com/trexem/Asteroids/blob/master/Asteroids/src/Timer.hpp) has a simple timer class, you can pause, resume and restart timer


## Try out
Download the `/release` folder and run `AsteroidsGame`

### TODO

 - [ ] asteroids class
 - [X] particle when moving ship
 - [X] window class
 - [X] renderer class
 - [X] GAME (god)class
 - [ ] Refactor Game class
