#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"
#include "shared_mutex_lock_priority.h"
#include "Helpers.h"

namespace Pong
{
	namespace GameEngine
	{
		using darknessNight::Multithreading::shared_mutex_lock_priority;

		class View
		{
		public:
			virtual void DisplayObjects(const std::vector<std::shared_ptr<GameObject>>& objects){}
		};

		class GameEngine
		{
		private:
			std::vector<std::shared_ptr<GameObject>> allObjects;
			shared_mutex_lock_priority objectsMutex;

		private:
			Corners getShiftedCorners(GameObject* obj, Pointf shift);
			Pointf* getCornersInArray(GameObject* obj, Pointf shift);
			Pointf getCenterPointBasedOnCorners(Corners corners);
			bool checkDeadzoneAndObjectsNewPosition(GameObject* obj, Pointf shift);
			bool checkNewPosition(GameObject* obj1, Corners cornersOfObject);
			bool checkIfPositionsAreEqual(GameObject* obj, Corners cornersOfOcject);

		public:
			bool WillCollide(std::shared_ptr<GameObject> obj, Pointf shift);
			bool MoveObject(std::shared_ptr<GameObject> obj, Pointf shift);

			std::shared_ptr<GameObject> CreateObject(Pointf pos, Pointf size, GameObject::Type type)
			{
				auto obj = std::make_shared<GameObject>(pos, size, type);
				AddObject(obj);
				return obj;
			}

			std::vector<std::shared_ptr<GameObject>> GetAllObjects()
			{
				std::shared_lock<shared_mutex_lock_priority> lock(objectsMutex);
				return allObjects;
			}

			void AddObject(std::shared_ptr<GameObject> obj)
			{
				std::lock_guard<shared_mutex_lock_priority> lock(objectsMutex);
				this->allObjects.push_back(obj);
			}

			virtual void DoScripts()
			{
				std::shared_lock<shared_mutex_lock_priority> lock(objectsMutex);
				for(auto obj:allObjects)
				{
					obj->DoScript();
				}
			}

			virtual void DoPhysic()
			{
				std::shared_lock<shared_mutex_lock_priority> lock(objectsMutex);
				for (auto obj : allObjects)
				{
					obj->DoPhysic();
				}
			}

			virtual void DoView(std::shared_ptr<View> view)
			{
				std::shared_lock<shared_mutex_lock_priority> lock(objectsMutex);
				view->DisplayObjects(allObjects);
			}
		};

		
	}
}
