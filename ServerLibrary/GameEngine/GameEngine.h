#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"
#include "shared_mutex_lock_priority.h"
#include "Helpers.h"
#include "View.h"

namespace Pong
{
	namespace GameEngine
	{
		using darknessNight::Multithreading::shared_mutex_lock_priority;

		class GameEngine
		{
		private:
			std::vector<std::shared_ptr<GameObject>> allObjects;
			shared_mutex_lock_priority objectsMutex;
			bool working;
			bool pause;
			std::vector<std::shared_ptr<std::thread>> threads;
			std::chrono::milliseconds viewCooldown, physicCooldown, scriptCooldown;

		private:
			Corners getShiftedCorners(GameObject* obj, Pointf shift);
			Pointf* getCornersInArray(GameObject* obj, Pointf shift);
			Pointf getCenterPointBasedOnCorners(Corners corners);
			bool checkDeadzoneAndObjectsNewPosition(GameObject* obj, Pointf shift);
			bool checkNewPosition(GameObject* obj1, Corners cornersOfObject);
			bool checkIfPositionsAreEqual(GameObject* obj, Corners cornersOfOcject);

		public:
			GameEngine()
			{
				viewCooldown = std::chrono::milliseconds(100);
				physicCooldown = std::chrono::milliseconds(10);
				scriptCooldown = std::chrono::milliseconds(10);
			}

			virtual ~GameEngine()
			{
				Stop();
			}

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

			void RemoveObject(std::shared_ptr<GameObject> obj)
			{
				std::lock_guard<shared_mutex_lock_priority> lock(objectsMutex);
				for(auto it=allObjects.begin();it!=allObjects.end();it++)
					if(*it==obj)
					{
						allObjects.erase(it);
						break;
					}
			}

		protected:
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

		public:
			void Start(std::shared_ptr<View> view)
			{
				working = true;
				pause = false;

				threads.push_back(std::make_shared<std::thread>([&]()
				{
					while (working && !pause)
					{
						DoPhysic();
						std::this_thread::sleep_for(physicCooldown);
					}
				}));

				threads.push_back(std::make_shared<std::thread>([&]()
				{
					while (working)
					{
						DoView(view);
						std::this_thread::sleep_for(viewCooldown);
					}
				}));

				threads.push_back(std::make_shared<std::thread>([&]()
				{
					while (working &&!pause)
					{
						DoScripts();
						std::this_thread::sleep_for(scriptCooldown);
					}
				}));
			}

			void Pause()
			{
				pause = true;
			}

			void Resume()
			{
				pause = false;
			}

			void Stop()
			{
				working = false;
				for (auto t : threads)
				{
					if (t->joinable())
						t->join();
				}
			}
		};

		
	}
}
