#pragma once
#include "GameEngine/GameObject.h"
#include "Consts.h"
#include <random>


namespace Pong
{
	class CommonBall :public GameEngine::GameObject
	{
		std::random_device eng;
		std::uniform_real_distribution<float> dist;
	public:
		CommonBall() :
			GameObject({ BoardWidth / 2, BoardHeight / 2 }, { Consts::BALL_RADIUS,Consts::BALL_RADIUS }, Type::BallCommon),
			dist(0.00001, MaxBallSpeed * 2)
		{
			Start();
		}
	private:
		void Start()
		{
			GameEngine::PhysicParams physic;
			do
			{
				physic.velocity.x = dist(eng) - MaxBallSpeed;
				physic.velocity.y = dist(eng) - MaxBallSpeed;
				physic.avgVelocity = sqrtf(physic.velocity.x*physic.velocity.x + physic.velocity.y*physic.velocity.y);
			} while (physic.avgVelocity < MinBallSpeed);

			SetPos({ BoardWidth / 2, BoardHeight / 2 });
			SetPhysic(physic);
		}

	public:
		void CollideAction(std::shared_ptr<GameObject> other) override
		{
			if (other->GetType() == Type::DeadWall)
			{
				Start();
			}
			else GameObject::CollideAction(other);
		}
	};
}
