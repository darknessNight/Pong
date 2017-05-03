#pragma once
#include "GameEngine/GameObject.h"
#include "Consts.h"
#include "PlayerObject.h"

namespace Pong
{
	class DeadWall:public GameEngine::GameObject
	{
		std::shared_ptr<PlayerObject> myPlayer;
	public:
		DeadWall(std::shared_ptr<PlayerObject> player)
			:GameObject(GetPosFromId(player->GetType()), GetSizeFromId(player->GetType()),
				Type::DeadWall), myPlayer(player)
		{}

	protected:
		static GameEngine::Pointf GetSizeFromId(Type id)
		{
			if (id == Internet::Player1)
			{
				return { Consts::BALL_RADIUS,BoardHeight*2 };
			}
			if (id == Internet::Player2)
			{
				return { BoardWidth *2,Consts::BALL_RADIUS };
			}
			if (id == Internet::Player3)
			{
				return { Consts::BALL_RADIUS,BoardHeight * 2 };
			}
			return {};
		}

		static GameEngine::Pointf GetPosFromId(Type id)
		{
			if (id == Internet::Player1)
			{
				return { BoardWidth+Consts::BALL_RADIUS,-BoardHeight / 2 };
			}
			if (id == Internet::Player2)
			{
				return { -BoardWidth / 2,BoardHeight+ Consts::BALL_RADIUS };
			}
			if (id == Internet::Player3)
			{
				return { -Consts::BALL_RADIUS, -BoardHeight / 2 };
			}
			return {};
		}

	public:
		void CollideAction(std::shared_ptr<GameObject> object) override{
			if (object->GetType() == Type::BallCommon)
				myPlayer->DecreaseLives();
		}
	};
}
