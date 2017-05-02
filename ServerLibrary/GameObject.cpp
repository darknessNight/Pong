#include"stdafx.h"
#include"GameEngine\GameObject.h"

Pong::GameEngine::GameObject::GameObject(Pointf pos, Pointf size, Type type)
{
	this->position = pos;
	this->size = size;
	this->type = type;
}
