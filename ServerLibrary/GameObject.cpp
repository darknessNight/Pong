#include"stdafx.h"
#include"GameEngine\GameObject.h"
#include "GameEngine/GameEngine.h"
#include <iostream>

Pong::GameEngine::GameObject::GameObject(Pointf pos, Pointf size, Type type)
{
	this->position = pos;
	this->size = size;
	this->type = type;
}

inline void Pong::GameEngine::GameObject::DoPhysic()
{
	if (lastPhysic == std::chrono::high_resolution_clock::time_point::min())
	{

	}
	else
	{
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastPhysic);
		Pointf shift = physic.velocity;
		shift.x /= duration.count();
		shift.y /= duration.count();

		/*auto pos = GetPos();
		pos.x += shift.x;
		pos.y += shift.y;
		SetPos(pos);*/
		engine->MoveObject(std::shared_ptr<GameObject>(this, [](void*) {}), shift);
	}
	lastPhysic = std::chrono::high_resolution_clock::now();
}
