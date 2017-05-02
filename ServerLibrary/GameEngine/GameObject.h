#pragma once
#include <exception>
#include <mutex>
#include <shared_mutex>
#include "Helpers.h"


namespace Pong
{
	namespace GameEngine
	{
		class GameObject
		{
		public:
			virtual ~GameObject() = default;

			enum Type
			{
				Test
			};
		private:
			friend class GameEngine;
			std::shared_ptr<std::shared_mutex> mutex=std::make_shared<std::shared_mutex>();
			Pointf position, size;
			Type type;

		protected:
			virtual void SetPos(Pointf p)
			{
				std::lock_guard<std::shared_mutex> lock(*mutex);
				position = p;
			}

			virtual void SetSize(Pointf s)
			{
				std::lock_guard<std::shared_mutex> lock(*mutex);
				size = s;
			}

			virtual void SetType(Type t)
			{
				std::lock_guard<std::shared_mutex> lock(*mutex);
				type = t;
			}
		public:
			GameObject(const GameObject& other)
			{
				mutex = std::make_shared<std::shared_mutex>();
				*this = other;
			}

			GameObject(Pointf pos, Pointf size, Type type);
			virtual Pointf GetPos()const
			{
				std::shared_lock<std::shared_mutex> lock(*mutex);
				return position;
			}

			virtual Pointf GetSize() const
			{
				std::shared_lock<std::shared_mutex> lock(*mutex);
				return size;
			}

			virtual Type GetType() const
			{
				std::shared_lock<std::shared_mutex> lock(*mutex);
				return Type::Test;
			}

			GameObject& operator=(const GameObject& other){
				std::lock_guard<std::shared_mutex> lock(*mutex);
				std::shared_lock<std::shared_mutex> lockOther(*other.mutex);
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

			virtual void DoScript()
			{
				
			}

			virtual void DoPhysic()
			{
				
			}
		};


	}
}
