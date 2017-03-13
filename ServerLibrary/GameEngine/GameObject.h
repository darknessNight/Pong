#pragma once
#include <exception>

namespace Pong
{
	namespace GameEngine
	{
		struct Pointf
		{
			float x, y;

			bool operator==(const Pointf& other) const
			{
				return x == other.x&&y == other.y;
			}
		};

		class GameObject
		{
		public:
			enum Type
			{
				Test
			};
		private:
			friend class GameEngine;

		protected:
			virtual void SetPos(Pointf){}//brak testów. trzeba dopisaæ
			virtual void SetSize(Pointf) {}
			virtual void SetType(Type) {}
		public:
			virtual Pointf GetPos() const { return{ 0,0 }; }
			virtual Pointf GetSize() const{ return{ 0,0 }; }
			virtual Type GetType() const { return Type::Test; }

			bool operator==(const GameObject& other) const
			{
				return other.GetPos() == GetPos() && other.GetSize() == GetSize() && other.GetType() == GetType();
			}

			bool operator!=(const GameObject& other) const
			{
				return !operator==(other);
			}
		};


	}
}