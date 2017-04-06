#include"stdafx.h"
#include "GameEngine\GameEngine.h"
#include "GameEngine\GameObject.h"
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

void Pong::GameEngine::GameEngine::addObject(std::shared_ptr<GameObject> obj)
{
	this->allObjects.push_back(obj);
}

std::shared_ptr<GameObject> Pong::GameEngine::GameEngine::CreateObject(Pointf pos, Pointf size, GameObject::Type type)
{
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(pos, size, type);
	addObject(obj);
	return obj;
}



bool Pong::GameEngine::GameEngine::MoveObject(std::shared_ptr<GameObject> obj, Pointf shift)
{
	if (!WillCollide(obj, shift)) {
		GameObject* object = obj.get();
		object->SetPos(shift + object->GetPos());
		return true;
	}

	return false;
}

bool Pong::GameEngine::GameEngine::WillCollide(std::shared_ptr<GameObject> obj, Pointf shift)
{
	GameObject* object = obj.get();
	return checkDeadzoneAndObjectsNewPosition(object, shift);
}

bool Pong::GameEngine::GameEngine::checkDeadzoneAndObjectsNewPosition(GameObject* obj, Pointf shift)
{
	GameObject* object;
	Corners cornersOfObject = getShiftedCorners(obj, shift);
	Pointf position = obj->GetPos();
	Pointf size = obj->GetSize();
	LinearFunctions_Deadzones DeadZone(position, size, shift);
	std::vector < std::shared_ptr<GameObject>> gameObjectsVector = GetAllObjects();

	for (unsigned int i = 0; i < gameObjectsVector.size(); i++)
	{
		object = gameObjectsVector.at(i).get();
		if (checkIfPositionsAreEqual(object, cornersOfObject))
			return true;

		if (checkNewPosition(object, cornersOfObject))
			return true;

		if (DeadZone.checkIfInDeadZone(cornersOfObject.lowerLeft))
			return true;
		if (DeadZone.checkIfInDeadZone(cornersOfObject.lowerRight))
			return true;
		if (DeadZone.checkIfInDeadZone(cornersOfObject.upperLeft))
			return true;
		if (DeadZone.checkIfInDeadZone(cornersOfObject.upperRight))
			return true;
	}

	return false;
}

Corners Pong::GameEngine::GameEngine::getShiftedCorners(GameObject* obj, Pointf shift)
{
	Corners corner;
	Pointf objectSize = obj->GetSize();

	corner.upperLeft = obj->GetPos() + shift;
	corner.lowerLeft = obj->GetPos() + shift;
	corner.upperRight = obj->GetPos() + shift;
	corner.lowerRight = obj->GetPos() + shift;

	corner.lowerLeft.y += objectSize.y;
	corner.upperRight.x += objectSize.x;
	corner.lowerRight.x += objectSize.x;
	corner.lowerRight.y = objectSize.y;

	return Corners(corner);
}

bool Pong::GameEngine::GameEngine::checkIfPositionsAreEqual(GameObject * obj, Corners cornersOfOcject)
{
	Pointf position = obj->GetPos();
	if (position == cornersOfOcject.upperLeft)
		return true;
	return false;
}

bool Pong::GameEngine::GameEngine::checkNewPosition(GameObject* obj1, Corners cornersOfObject)
{
	int framedPoints;
	bool retVal = false;
	Pointf zeroPoint;
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

Pointf* Pong::GameEngine::GameEngine::getCornersInArray(GameObject * obj, Pointf shift)
{
	Corners corner = getShiftedCorners(obj, shift);

	Pointf* pointsArray = new Pointf[4];

	pointsArray[0] = corner.upperLeft;
	pointsArray[1] = corner.upperRight;
	pointsArray[2] = corner.lowerLeft;
	pointsArray[3] = corner.lowerRight;

	return pointsArray;
}







