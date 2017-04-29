#pragma once
#include <exception>


namespace Pong
{
	namespace GameEngine
	{
		struct Pointf
		{
			Pointf(float x, float y) {
				this->x = x;
				this->y = y;
			}
			Pointf() = default;

			float x = 0;
			float y = 0;

			bool operator==(const Pointf& other) const
			{
				return x == other.x && y == other.y;
			}

			friend Pointf operator+(const Pointf& left, const Pointf& right) {
				return Pointf(left.x + right.x, left.y + right.y);
			}

			Pointf operator+=(const Pointf& other) const {
				Pointf newPointf(other);
				newPointf.x += other.x;
				newPointf.y += other.y;
				return newPointf;
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
			Pointf position, size;
			Type type;

		protected:
			virtual void SetPos(Pointf p) { position = p; }//brak testów. trzeba dopisaæ
			virtual void SetSize(Pointf s) { size = s; }
			virtual void SetType(Type t) { type = t; }
		public:
			GameObject(Pointf pos, Pointf size, Type type);
			virtual Pointf GetPos() const { return position; }
			virtual Pointf GetSize() const{ return size; }
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