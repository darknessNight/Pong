#pragma once
#include "GameObject.h"
#include <vector>

namespace Pong {
	namespace GameEngine {
		class View
		{
		public:
			virtual void DisplayObjects(const std::vector<std::shared_ptr<Pong::GameEngine::GameObject>>& objects) {}
		};
	}
}