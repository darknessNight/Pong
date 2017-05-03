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
			bool working = false;
			bool pause = false;
			std::vector<std::shared_ptr<std::thread>> threads;
			std::chrono::milliseconds viewCooldown, physicCooldown, scriptCooldown;

		private:
			Corners getShiftedCorners(std::shared_ptr<GameObject> obj, Pointf shift);
			Pointf* getCornersInArray(std::shared_ptr<GameObject> obj, Pointf shift);
			Pointf getCenterPointBasedOnCorners(Corners corners);
			std::shared_ptr<GameObject> checkDeadzoneAndObjectsNewPosition(std::shared_ptr<GameObject> obj, Pointf shift);
			bool checkNewPosition(std::shared_ptr<GameObject> obj1, Corners cornersOfObject);
			bool checkIfPositionsAreEqual(std::shared_ptr<GameObject> obj, Corners cornersOfOcject);

		public:
			GameEngine()
			{
				viewCooldown = std::chrono::milliseconds(1);
				physicCooldown = std::chrono::milliseconds(10);
				scriptCooldown = std::chrono::milliseconds(10);
			}

			virtual ~GameEngine()
			{
				Stop();
			}

			bool WillCollide(std::shared_ptr<GameObject> obj, Pointf shift);
			bool CheckWillObjectsCollide(Corners cornersOfObject, LinearFunctions_Deadzones DeadZone, std::vector<std::shared_ptr<GameObject>>::value_type object);
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
				obj->SetEngine(this);
				std::lock_guard<shared_mutex_lock_priority> lock(objectsMutex);
				this->allObjects.push_back(obj);
			}

			void RemoveObject(std::shared_ptr<GameObject> obj)
			{
				std::lock_guard<shared_mutex_lock_priority> lock(objectsMutex);
				for (auto it = allObjects.begin(); it != allObjects.end(); it++)
					if (*it == obj)
					{
						allObjects.erase(it);
						break;
					}
			}

		protected:
			virtual void DoScripts()
			{
				std::vector<std::shared_ptr<GameObject>> objs;
				{
					std::shared_lock<shared_mutex_lock_priority> lock(objectsMutex);
					objs = allObjects;
				}
				for (auto obj : objs)
				{
					obj->DoScript();
				}
			}

			virtual void DoPhysic()
			{
				std::vector<std::shared_ptr<GameObject>> objs;
				{
					std::shared_lock<shared_mutex_lock_priority> lock(objectsMutex);
					objs = allObjects;
				}
				for (auto obj : objs)
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
				if (working) return;
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

				threads.push_back(std::make_shared<std::thread>([&, view]()
				{
					while (working)
					{
						DoView(view);
						std::this_thread::sleep_for(viewCooldown);
					}
				}));

				threads.push_back(std::make_shared<std::thread>([&]()
				{
					while (working && !pause)
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
