#pragma once
#include "GameEngine/GameObject.h"
#include "Consts.h"
#include <random>


namespace Pong
{
	class CommonBall:public GameEngine::GameObject
	{
	public:
		CommonBall():
		GameObject({BoardWidth/2, BoardHeight/2},{Consts::BALL_RADIUS,Consts::BALL_RADIUS},Type::BallCommon)
		{
			Start();
		}
	private:
		void Start()
		{
			std::default_random_engine eng;
			std::uniform_real_distribution<float> dist(0.00001, MaxBallSpeed*2);

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
			if(other->GetType()==Type::DeadWall)
			{
				Start();
			}
			else GameObject::CollideAction(other);
		}
	};
}
