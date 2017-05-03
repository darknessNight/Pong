#pragma once
#include "InternetProtocol/ConstantsAndStructs.h"
#include "GameEngine/GameObject.h"

namespace Pong {

	class BadBall :public GameEngine::GameObject
	{
		std::function<void(std::shared_ptr<GameEngine::GameObject>)> removeObjFunc;
	public:
		BadBall(GameEngine::Pointf pos,
			std::function<void(std::shared_ptr<GameEngine::GameObject>)> removeFunc)
			:GameObject(pos, { Consts::BALL_RADIUS, Consts::BALL_RADIUS }, Type::BallRed),
			removeObjFunc(removeFunc)
		{
			lives = BadBallLives;
		}

		bool IsCollideWith(std::shared_ptr<GameObject> other) override
		{
			switch (other->GetType())
			{
			case Type::BallCommon:
			case Type::Test:
			case Type::BallRed:
				return false;
			default:
				return true;
			}
		}

		void CollideAction(std::shared_ptr<GameObject> other) override {
			if(other->GetType()>=Type::Player1 && other->GetType()<=Type::Player3)
			{
				if (other->IsCollideWith(std::shared_ptr<GameObject>(this, [](GameObject*) {}))) {
					other->DecreaseLives();
					KillObject();
				}
			}
			GameObject::CollideAction(other);
		}

		void KillObject() override
		{
			removeObjFunc(std::shared_ptr<GameObject>(this, [](GameObject*) {}));
		}
	};
}
