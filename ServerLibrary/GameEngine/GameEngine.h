#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"

namespace Pong
{
	namespace GameEngine
	{
		class GameEngine
		{
		public:
			std::shared_ptr<GameObject> CreateObject(Pointf pos, Pointf size, GameObject::Type type)
			{
				return nullptr;
			}

			bool WillCollide(std::shared_ptr<GameObject> obj, Pointf shift)
			{
				throw std::exception("Not implement!");
				return false;
			}

			bool MoveObject(std::shared_ptr<GameObject> obj, Pointf shift)
			{
				return false;
			}

			std::vector<std::shared_ptr<GameObject>> GetAllObjects()
			{
				return std::vector<std::shared_ptr<GameObject>>();
			}
		};
	}
}
