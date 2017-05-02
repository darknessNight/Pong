#pragma once
#include "GameEngine/GameObject.h"

namespace Pong
{
	class PlayerObject:public GameEngine::GameObject
	{
	public:
		bool IsShielded() const
		{
			std::shared_lock<std::shared_mutex> lock(*changeMutex);
			return false;
		}
	};
}