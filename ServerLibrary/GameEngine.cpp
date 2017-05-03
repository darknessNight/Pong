#include"stdafx.h"
#include "GameEngine\GameEngine.h"
#include "GameEngine\GameObject.h"
#include "GameEngine/Helpers.h"

using Pong::GameEngine::Pointf;

//
/*
		^ -y
		|
		|
-x		|		x
<--------------->-------------------.
		|							|
		|							|
		|			SCREEN			|
		V y							|
		|							|
		|							|
		|							|
		|___________________________|
*/

using namespace Pong::GameEngine;

bool  GameEngine::MoveObject(std::shared_ptr<GameObject> obj, Pointf shift)
{
	auto result = checkDeadzoneAndObjectsNewPosition(obj, shift);
	if (result==nullptr) {
		GameObject* object = obj.get();
		object->SetPos(shift + object->GetPos());
		return true;
	}else
	{
		obj->CollideAction(result);
		result->CollideAction(obj);
	//TODO przyda³oby siê dosuniêcie do obiektu z którym koliduje	
	}
	return false;
}

bool Pong::GameEngine::GameEngine::WillCollide(std::shared_ptr<GameObject> obj, Pointf shift)
{
	return checkDeadzoneAndObjectsNewPosition(obj, shift)!=nullptr;
}

std::shared_ptr<GameObject> Pong::GameEngine::GameEngine::checkDeadzoneAndObjectsNewPosition(std::shared_ptr<GameObject> obj, Pointf shift)
{
	auto cornersOfObject = getShiftedCorners(obj, shift);
	auto position = obj->GetPos();
	auto size = obj->GetSize();
	LinearFunctions_Deadzones DeadZone(position, size, shift);
	auto gameObjectsVector = GetAllObjects();

	for (auto object:gameObjectsVector)
	{
		if (object == obj) {
			continue;
		}
		bool value1;
		if (CheckWillObjectsCollide(cornersOfObject, DeadZone, object))
		{
			if(object->IsCollideWith(obj) && obj->IsCollideWith(object))
			return object;
		}
			
	}

	return nullptr;
}

bool GameEngine::CheckWillObjectsCollide(Corners cornersOfObject, LinearFunctions_Deadzones DeadZone, std::shared_ptr<GameObject> object)
{
	Corners cornersOfCheckedOcject = getShiftedCorners(object, { 0,0 });
	if (checkIfPositionsAreEqual(object, cornersOfObject))
	{
		return true;
	}

	if (checkNewPosition(object, cornersOfObject))
	{
		return true;
	}


	if (DeadZone.checkIfPointIsInDeadZone(getCenterPointBasedOnCorners(cornersOfCheckedOcject)))
	{
		return true;
	}

	if (DeadZone.checkIfPointIsInDeadZone(cornersOfCheckedOcject.upperLeft))//was cornderOfObject before
	{
		return true;
	}
	if (DeadZone.checkIfPointIsInDeadZone(cornersOfCheckedOcject.upperRight))
	{
		return true;
	}
	if (DeadZone.checkIfPointIsInDeadZone(cornersOfCheckedOcject.lowerLeft))
	{
		return true;
	}
	if (DeadZone.checkIfPointIsInDeadZone(cornersOfCheckedOcject.lowerRight))
	{
		return true;
	}

	if (DeadZone.checkIfDeadZoneOverlapsObject(cornersOfCheckedOcject))
	{
		return true;
	}
	return false;
}

Corners Pong::GameEngine::GameEngine::getShiftedCorners(std::shared_ptr<GameObject> obj, Pointf shift)
{
	Corners corner;
	Pointf objectSize = obj->GetSize();

	corner.upperLeft = obj->GetPos() + shift;
	corner.lowerLeft = obj->GetPos() + shift+ Pointf{0, obj->GetSize().y};
	corner.upperRight = obj->GetPos() + shift + Pointf{ obj->GetSize().x, 0 };
	corner.lowerRight = obj->GetPos()+obj->GetSize() + shift;

	return Corners(corner);
}

bool Pong::GameEngine::GameEngine::checkIfPositionsAreEqual(std::shared_ptr<GameObject> obj, Corners cornersOfOcject)
{
	Pointf position = obj->GetPos();
	if (position == cornersOfOcject.upperLeft)
		return true;
	return false;
}

bool Pong::GameEngine::GameEngine::checkNewPosition(std::shared_ptr<GameObject> obj1, Corners cornersOfObject)
{
	int framedPoints;
	bool retVal = false;
	Pointf zeroPoint={0,0};
	Pointf* cornersArray = getCornersInArray(obj1, zeroPoint);

	for (int i = 0; i < 4; i++) {
		framedPoints = 0;
		if (cornersArray[i].x > cornersOfObject.upperLeft.x &&
			cornersArray[i].y > cornersOfObject.upperLeft.y)
			framedPoints++;
		if (cornersArray[i].x < cornersOfObject.upperRight.x &&
			cornersArray[i].y > cornersOfObject.upperRight.y)
			framedPoints++;
		if (cornersArray[i].x > cornersOfObject.lowerLeft.x &&
			cornersArray[i].y < cornersOfObject.lowerLeft.y)
			framedPoints++;
		if (cornersArray[i].x < cornersOfObject.lowerRight.x &&
			cornersArray[i].y < cornersOfObject.lowerRight.y)
			framedPoints++;

		if (framedPoints == 4) {
			retVal = true;
			break;
		}
	}

	delete[] cornersArray;
	return retVal;
}

Pointf* Pong::GameEngine::GameEngine::getCornersInArray(std::shared_ptr<GameObject> obj, Pointf shift)
{
	Corners corner = getShiftedCorners(obj, shift);

	Pointf* pointsArray = new Pointf[4];

	pointsArray[0] = corner.upperLeft;
	pointsArray[1] = corner.upperRight;
	pointsArray[2] = corner.lowerLeft;
	pointsArray[3] = corner.lowerRight;

	return pointsArray;
}

Pointf Pong::GameEngine::GameEngine::getCenterPointBasedOnCorners(Corners corners)
{
	float diffX, diffY;
	diffX = corners.upperRight.x - corners.upperLeft.x;
	diffY = corners.lowerLeft.y - corners.upperLeft.y;
	Pointf centerPoint;
	centerPoint.x = corners.upperLeft.x + diffX/2;
	centerPoint.y = corners.upperLeft.y + diffY/2;
	return Pointf(centerPoint);
}






