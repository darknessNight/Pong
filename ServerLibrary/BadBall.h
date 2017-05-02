#pragma once
#include "InternetProtocol/ConstantsAndStructs.h"
#include "GameEngine/GameObject.h"

namespace Pong{

	class BadBall :public GameEngine::GameObject
	{
	public:
		BadBall(GameEngine::Pointf pos) :GameObject(pos, { Consts::BALL_RADIUS, Consts::BALL_RADIUS }, Type::BallRed)
		{

		}
	};
}
