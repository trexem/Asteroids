#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "texture.hpp"
#include "utils.hpp"voi

class GameObject
{
public:
	GameObject();
	GameObject(Pos p);
	~GameObject();

	void render(void);
	void setPos(Pos p);
	Pos getPos();

private:
	Pos m_pos;
	Texture* m_texture;
};

#endif // !_GAMEOBJECT_H_