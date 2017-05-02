#pragma once

namespace Pong {
	namespace GameEngine {
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

			Pointf& operator+=(const Pointf& other) {
				x += other.x;
				y += other.y;
				return *this;
			}

		};


		struct Corners {
			Pointf upperLeft = { 0,0 };
			Pointf upperRight = { 0,0 };
			Pointf lowerLeft = { 0,0 };
			Pointf lowerRight = { 0,0 };
		};

		class LinearFunctions_Deadzones
		{
		public:

			struct FunctionDomain {
				float from = -INFINITY;
				float to = INFINITY;
			};
			enum FunctionTypes {
				RISING,
				FALLING,
				HORIZONTAL,
				VERTICAL,
				NONE
			};
			typedef int FunctionType;

			LinearFunctions_Deadzones(Pong::GameEngine::Pointf pos, Pointf size, Pointf shift);
			LinearFunctions_Deadzones();

			void setNewFunktionVariables(Pointf pos, Pointf size, Pointf shift);

			/*
			WARNING! F1 and F2 return x instead of y regardless of the passed
			argument when functionType is VERTICAL!
			*/
			float F1(float x);
			float F2(float x);

			bool checkIfPointIsInDeadZone(Pointf pos);
			bool checkIfDeadZoneOverlapsObject(Corners corners);
			bool pointIsWithinDomain(float x);

		private:
			float a;
			float x;
			float b1, b2;
			float y; //=a*x+b

			FunctionDomain Domain;
			FunctionType functionType;

			int determineFunctionType(Pointf shift);
		};
	}
}