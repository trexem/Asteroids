#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "texture.hpp"
#include "utils.hpp"
#include "FPair.h"

class GameObject
{
public:
	GameObject();
	GameObject(FPair p);
	~GameObject();

	void renderEx(void);
	void render(void);
	void setPos(FPair t_pos, double t_rot_degrees);
	FPair getPos();
	double getX();
	double getY();
	double getDegrees();
	SDL_Rect getCollider();

protected:
	FPair m_pos{0,0};
	double m_rot_degrees{ 0 };
	Texture* m_texture;
	SDL_Rect m_collider;
};

#endif // !_GAMEOBJECT_H_