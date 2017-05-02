#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"

namespace Pong
{
	namespace GameEngine
	{

		struct Corners {
			Pointf upperLeft = { 0,0 };
			Pointf upperRight = { 0,0 };
			Pointf lowerLeft = { 0,0 };
			Pointf lowerRight = { 0,0 };
		};

		class GameEngine
		{
		private:
			std::vector<std::shared_ptr<GameObject>> allObjects;

			Corners getShiftedCorners(GameObject* obj, Pointf shift);
			Pointf* getCornersInArray(GameObject* obj, Pointf shift);
			Pointf getCenterPointBasedOnCorners(Corners corners);
			bool checkDeadzoneAndObjectsNewPosition(GameObject* obj, Pointf shift);
			bool checkNewPosition(GameObject* obj1, Corners cornersOfObject);
			bool checkIfPositionsAreEqual(GameObject* obj, Corners cornersOfOcject);

		public:

			void addObject(std::shared_ptr<GameObject> obj);
			std::shared_ptr<GameObject> CreateObject(Pointf pos, Pointf size, GameObject::Type type);
			bool WillCollide(std::shared_ptr<GameObject> obj, Pointf shift);
			bool MoveObject(std::shared_ptr<GameObject> obj, Pointf shift);
			std::vector<std::shared_ptr<GameObject>> GetAllObjects() { return allObjects; }
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

			LinearFunctions_Deadzones(Pointf pos, Pointf size, Pointf shift);
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
