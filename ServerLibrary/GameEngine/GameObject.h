#pragma once
#include <exception>
#include <mutex>
#include <shared_mutex>
#include "Helpers.h"
#include "../InternetProtocol/ConstantsAndStructs.h"

namespace Pong
{
	namespace GameEngine
	{
		class GameObject
		{
		public:
			virtual ~GameObject() = default;

			typedef Internet::ConnectionObject::Type Type;
		private:
			friend class GameEngine;
			std::shared_ptr<std::shared_mutex> changeMutex=std::make_shared<std::shared_mutex>();
			std::shared_ptr<std::shared_mutex> positionMutex = std::make_shared<std::shared_mutex>();
			Pointf position, size;
			Type type;
			PhysicParams physic;
			int lives=1;
		protected:
			virtual void SetPos(Pointf p)
			{
				std::lock_guard<std::shared_mutex> lock(*changeMutex);
				position = p;
			}

			virtual void SetSize(Pointf s)
			{
				std::lock_guard<std::shared_mutex> lock(*changeMutex);
				size = s;
			}

			virtual void SetType(Type t)
			{
				std::lock_guard<std::shared_mutex> lock(*changeMutex);
				type = t;
			}

			virtual void SetPhysic(const PhysicParams& params)
			{
				std::lock_guard<std::shared_mutex> lock(*changeMutex);
				physic = params;
			}

			virtual void SetLives(int lives)
			{
				std::lock_guard<std::shared_mutex> lock(*changeMutex);
				this->lives = lives;
			}
		public:
			GameObject(const GameObject& other)
			{
				changeMutex = std::make_shared<std::shared_mutex>();
				positionMutex = std::make_shared<std::shared_mutex>();
				*this = other;
			}

			GameObject(Pointf pos, Pointf size, Type type);
			virtual Pointf GetPos()const
			{
				std::shared_lock<std::shared_mutex> lock(*changeMutex);
				std::shared_lock<std::shared_mutex> lock2(*positionMutex);
				return position;
			}

			virtual Pointf GetSize() const
			{
				std::shared_lock<std::shared_mutex> lock(*changeMutex);
				std::shared_lock<std::shared_mutex> lock2(*positionMutex);
				return size;
			}

			virtual Type GetType() const
			{
				std::shared_lock<std::shared_mutex> lock(*changeMutex);
				return Type::Test;
			}

			virtual int GetLives() const
			{
				std::shared_lock<std::shared_mutex> lock(*changeMutex);
				return lives;
			}

			virtual void DecreaseLives()
			{
				std::lock_guard<std::shared_mutex> lock(*changeMutex);
				lives--;
				if (lives == 0)
					KillObject();
			}

			GameObject& operator=(const GameObject& other){
				std::lock_guard<std::shared_mutex> lock(*changeMutex);
				std::shared_lock<std::shared_mutex> lockOther(*other.changeMutex);
				size = other.size;
				position = other.position;
				type = other.type;
				return *this;
			}

			bool operator==(const GameObject& other) const
			{
				return other.GetPos() == GetPos() && other.GetSize() == GetSize() && other.GetType() == GetType();
			}

			bool operator!=(const GameObject& other) const
			{
				return !operator==(other);
			}

			virtual void KillObject()
			{
				
			}

			virtual bool IsCollideWith(std::shared_ptr<GameObject> otherObj)
			{
				return true;
			}

			virtual void CollideAction(std::shared_ptr<GameObject> object)
			{
				
			}

			virtual void DoScript()
			{
				
			}

			virtual void DoPhysic()
			{
				std::shared_lock<std::shared_mutex> lock(*positionMutex);

			}
		};


	}
}
