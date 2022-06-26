#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "texture.hpp"
#include "utils.hpp"

class GameObject
{
public:
	GameObject();
	GameObject(Pos p);
	~GameObject();

	void renderEx(void);
	void render(void);
	void setPos(Pos t_pos, double t_rot_degrees);
	Pos getPos();
	double getX();
	double getY();
	double getDegrees();

protected:
	Pos m_pos{0,0};
	double m_rot_degrees{ 0 };
	Texture* m_texture;
};

#endif // !_GAMEOBJECT_H_