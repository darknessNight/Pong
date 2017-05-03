#pragma once
#include "GameEngine/GameObject.h"
#include "Consts.h"
#include <random>
#include "BadBall.h"

namespace Pong
{
	class PlayerObject :public GameEngine::GameObject
	{
		std::shared_ptr<Internet::ServerUserPartConnection> connection;
		const float moveSpeed = PlayerMoveSpeed;
		bool horizontalMove;
		int direction;
		std::chrono::high_resolution_clock::time_point lastShield = std::chrono::high_resolution_clock::time_point::min();
		std::chrono::high_resolution_clock::time_point lastShoot = std::chrono::high_resolution_clock::time_point::min();

		std::chrono::milliseconds shieldCooldown = PlayerShieldCooldown;
		std::chrono::milliseconds shootCooldown = PlayerShootCooldown;
		bool shielded = true;
		std::chrono::milliseconds shieldTime = PlayerShieldTime;

		std::function<void(std::shared_ptr<GameEngine::GameObject>)> addObjFunc;
		std::function<void(std::shared_ptr<GameEngine::GameObject>)> removeObjFunc;
	public:
		PlayerObject(std::shared_ptr<Internet::ServerUserPartConnection> connection,
			Internet::UserIds id,
			std::function<void(std::shared_ptr<GameEngine::GameObject>)> addFunc,
			std::function<void(std::shared_ptr<GameEngine::GameObject>)> removeFunc)
			:GameObject(GetPosFromId(id), GetSizeFromId(id), static_cast<Type>(id)),
			connection(connection), addObjFunc(addFunc), removeObjFunc(removeFunc)
		{
			if (id == Internet::Player1)
			{
				horizontalMove = false;
				direction = -1;
			}
			if (id == Internet::Player2)
			{
				horizontalMove = true;
				direction = 1;
			}
			if (id == Internet::Player3)
			{
				horizontalMove = false;
				direction = 1;
			}

			GameObject::SetLives(PlayerInitLives);
		}

		static GameEngine::Pointf GetPosFromId(Internet::UserIds id)
		{
			if (id == Internet::Player1)
			{
				return { BoardWidth - Consts::BALL_RADIUS,BoardHeight / 2 - Consts::PLAYER_WIDTH / 2 };
			}
			if (id == Internet::Player2)
			{
				return { BoardWidth / 2 - Consts::PLAYER_WIDTH / 2,BoardHeight - Consts::BALL_RADIUS };
			}
			if (id == Internet::Player3)
			{
				return { 0,BoardHeight / 2 - Consts::PLAYER_WIDTH / 2 };
			}
			return {};
		}

		static GameEngine::Pointf GetSizeFromId(Internet::UserIds id)
		{
			if (id == Internet::Player1)
			{
				return { Consts::BALL_RADIUS,Consts::PLAYER_WIDTH };
			}
			if (id == Internet::Player2)
			{
				return { Consts::PLAYER_WIDTH,Consts::BALL_RADIUS };
			}
			if (id == Internet::Player3)
			{
				return { Consts::BALL_RADIUS,Consts::PLAYER_WIDTH };
			}
			return {};
		}

		bool IsShielded() const
		{
			std::shared_lock<std::shared_mutex> lock(*changeMutex);
			return false;
		}

		bool IsCollideWith(std::shared_ptr<GameObject> other) override
		{
			if (IsShielded() && other->GetType() == Type::BallRed)
				return false;
			return true;
		}

		void KillObject() override
		{
			std::shared_ptr<GameObject> wall;
			switch (GetType())
			{
			case Type::Player3:
			case Type::Player1:
				wall = std::make_shared<GameObject>(GameEngine::Pointf{ GetPos().x,0 },
					GameEngine::Pointf{ GetSize().x,BoardHeight }, Type::Wall);
				break;
			case Type::Player2:
				wall = std::make_shared<GameObject>(GameEngine::Pointf{ 0, GetPos().y },
					GameEngine::Pointf{ BoardWidth,GetSize().y }, Type::Wall);
				break;
			}
			addObjFunc(wall);
		}

		void DoScript() override {
			if (std::chrono::high_resolution_clock::now() - lastShield > shieldTime)
				shielded = false;

			auto actions = connection->GetActions();
			if (actions.size() > 0)
			{
				for (auto el : actions)
				{
					switch (el.moveType)
					{
					case Internet::Shield:
						MakeShield();
						break;
					case Internet::Shoot:
						MakeShoot();
						break;
					case Internet::StartMoveLeft:
						MoveLeft();
						break;
					case Internet::StartMoveRight:
						MoveRight();
						break;
					case Internet::StopMoveLeft:
						StopLeft();
						break;
					case Internet::StopMoveRight:
						StopRight();
						break;
					}
				}
			}
		}

	protected:
		void MakeShield()
		{
			if (std::chrono::high_resolution_clock::now() - lastShield > shieldCooldown)
			{
				lastShield = std::chrono::high_resolution_clock::now();
				shielded = true;
			}
		}

		void MakeShoot()
		{
			if (std::chrono::high_resolution_clock::now() - lastShield > shieldCooldown)
			{
				lastShield = std::chrono::high_resolution_clock::now();

				std::shared_ptr<BadBall> ball = PrepareRedBall();

				addObjFunc(ball);
			}
		}

		std::shared_ptr<BadBall> PrepareRedBall() const
		{
			GameEngine::Pointf pos, velocity;
			PreparePosAndVelocityForRedBall(pos, velocity);
			auto ball = std::make_shared<BadBall>(pos, removeObjFunc);
			ball->SetPhysic({ velocity, sqrtf(velocity.x*velocity.x + velocity.y*velocity.y) });
			return ball;
		}

		void PreparePosAndVelocityForRedBall(GameEngine::Pointf& pos, GameEngine::Pointf& velocity) const
		{
			std::default_random_engine random;
			std::uniform_real_distribution<float> dist(0.001, MaxBallSpeed);
			if (horizontalMove)
			{
				pos = GetPos();
				pos.y -= 2 * size.y;
				pos.x += size.x / 2;
				velocity.x = MaxBallSpeed - dist(random) / 2;
				velocity.y = -dist(random);
			}
			else
			{
				pos = GetPos();
				pos.y += size.y / 2;
				pos.x -= size.x * 2 * direction;
				velocity.x = -dist(random)*direction;
				velocity.y = MaxBallSpeed - dist(random) / 2;
			}
		}

		void MoveLeft()
		{
			std::cout << "Player: " << GetType() << " MoveLeft \n";
			if (horizontalMove)
				SetPhysic({ { -moveSpeed*direction,0 },0 });
			else SetPhysic({ { 0, -moveSpeed*direction },0 });
		}

		void MoveRight()
		{
			std::cout << "Player: " << GetType() << " MoveRight \n";
			if (horizontalMove)
				SetPhysic({ { moveSpeed*direction,0 },0 });
			else SetPhysic({ { 0, moveSpeed*direction },0 });
		}

		void StopLeft()
		{
			std::cout << "Player: " << GetType() << " StopLeft \n";
			StopRight();
		}

		void StopRight()
		{
			std::cout << "Player: " << GetType() << " StopRight \n";
			SetPhysic({ {0,0},0 });
		}
	};
}

