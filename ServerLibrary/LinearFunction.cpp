#include"stdafx.h"
#include "GameEngine/GameEngine.h"

using namespace Pong::GameEngine;

LinearFunctions_Deadzones::LinearFunctions_Deadzones(Pointf pos, Pointf size, Pointf shift)
{
	this->setNewFunctionVariables(pos, size, shift);
	this->y = 0;
	this->x = 0;
}

LinearFunctions_Deadzones::LinearFunctions_Deadzones()
{
	this->a = 0;
	this->b1 = 0;
	this->b2 = 0;
	this->y = 0;
	this->x = 0;
}

void LinearFunctions_Deadzones::setNewFunctionVariables(Pointf pos, Pointf size, Pointf shift)
{
	this->functionType = determineFunctionType(shift);
	if (functionType == HORIZONTAL) {	//y1 = b1; y2 = b2
		this->a = 0;
		this->b1 = size.y + pos.y;
		this->b2 = pos.y;
	}
	else if (functionType == VERTICAL) {	//technically this is not a function 
		this->a = 0;
		this->b1 = size.x + pos.x;
		this->b2 = pos.x;
	}
	else if (functionType == RISING) {
		this->a = (shift.y / shift.x);
		b1 = (size.y + pos.y) - (a*pos.x);
		b2 = pos.y - (a*(pos.x + size.x));

	}
	else if (functionType == FALLING) {
		this->a = (shift.y / shift.x);
		b1 = (pos.y + size.y) - (a*(pos.x + size.x));
		b2 = pos.y - (a*pos.x);
	}
	else
	{
		this->a = 0;
		this->b1 = 0;
		this->b2 = 0;
	}
	
	if (functionType != VERTICAL) {
		if (shift.x >= 0) {
			Domain.to = pos.x + shift.x;
			Domain.from = pos.x;
		}
		else
		{
			Domain.from = pos.x + shift.x;
			Domain.to = pos.x;
		}
	}
	else if (functionType == VERTICAL) {	//The Domain will actually be an Antidomain from this point
		if (shift.y >= 0) {
			Domain.to = pos.y + shift.y;
			Domain.from = pos.y;
		}
		else
		{
			Domain.from = pos.y + shift.y;
			Domain.to = pos.y;
		}
	}
}

float LinearFunctions_Deadzones::F1(float x)
{
	if (functionType == NONE)
		return NAN;
	else if (functionType == HORIZONTAL || functionType == VERTICAL)
		return b1;
	else
		return a*x + b1;
	return NAN;
}

float LinearFunctions_Deadzones::F2(float x)
{
	if (functionType == NONE)
		return NAN;
	else if (functionType == HORIZONTAL || functionType == VERTICAL)
		return b2;
	else
		return a*x + b2;
	return NAN;
}


bool LinearFunctions_Deadzones::checkIfPointIsInDeadZone(Pointf pos)
{
	if (functionType == NONE)
		return false;
	if (functionType != VERTICAL && pointIsWithinDomain(pos.x)) {
		float y1 = F1(pos.x);	//F1 returns the lower (coordination-wise) higher coord.
		float y2 = F2(pos.x);	//F2 returns the higher (coordination-wise) lower coord.
		if (pos.y <= y1 && pos.y >= y2)
			return true;
	}
	else if(functionType == VERTICAL && pointIsWithinDomain(pos.y)){
		float x1 = F1(pos.y);	//in this case F1 returns the right coord.
		float x2 = F2(pos.y);	//in this case F2 returns the left coord.
		if (pos.x <= x1 && pos.x >= x2)
			return true;
	}
	return false;
}

bool LinearFunctions_Deadzones::checkIfDeadZoneOverlapsObject(Corners corners)
{
	if (functionType == NONE)
		return false;
	if (functionType != VERTICAL) {
		float x;
		if (pointIsWithinDomain(corners.upperLeft.x))
			x = corners.upperLeft.x;
		else if (pointIsWithinDomain(corners.lowerLeft.x))
			x = corners.lowerLeft.x;
		else
			return false;

		float y1 = F1(x);	//F1 returns the lower (coordination-wise) higher coord.
		float y2 = F2(x);	//F2 returns the higher (coordination-wise) lower coord.
		if (corners.upperLeft.y < y2 && corners.lowerLeft.y >= y1)
			return true;
		if (corners.upperRight.y < y2 && corners.lowerRight.y >= y1)
			return true;
	}
	else
	{
		float y;
		if (pointIsWithinDomain(corners.upperLeft.y))
			y = corners.upperLeft.x;
		else if (pointIsWithinDomain(corners.lowerLeft.y))
			y = corners.lowerLeft.x;
		else
			return false;
		float x1 = F1(y);	//in this case F1 returns the right coord.
		float x2 = F2(y);	//in this case F2 returns the left coord.
		if (corners.upperLeft.x <= x2 && corners.upperRight.x >= x1)
			return true;
		if (corners.lowerLeft.x <= x2 && corners.lowerRight.x >= x1)
			return true;
	}
	return false;
}

bool LinearFunctions_Deadzones::pointIsWithinDomain(float x)
{
	if (x > Domain.from && x < Domain.to)
		return true;
	return false;
}

int LinearFunctions_Deadzones::determineFunctionType(Pointf shift)
{
	if ((shift.x > 0 && shift.y > 0) ||
		(shift.x < 0 && shift.y < 0))
		return RISING;
	if ((shift.x > 0 && shift.y < 0) ||
		(shift.x < 0 && shift.y > 0))
		return FALLING;
	if (shift.x == 0 && shift.y != 0)
		return VERTICAL;
	if (shift.y == 0 && shift.x != 0)
		return HORIZONTAL;
	
	return NONE;
}

