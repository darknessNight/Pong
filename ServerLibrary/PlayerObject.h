#pragma once
#include "GameEngine/GameObject.h"
#include "Consts.h"
#include <random>
#include "BadBall.h"

namespace Pong
{
	class PlayerObject :public GameEngine::GameObject
	{
		std::random_device random;
		std::uniform_real_distribution<float> dist;
		std::shared_ptr<Internet::ServerUserPartConnection> connection;
		const float moveSpeed = PlayerMoveSpeed;
		bool horizontalMove;
		int direction;
		std::chrono::high_resolution_clock::time_point lastShield = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::time_point lastShoot = std::chrono::high_resolution_clock::now();

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
			std::function<void(std::shared_ptr<GameEngine::GameObject>)> removeFunc) :
			GameObject(GetPosFromId(id), GetSizeFromId(id), static_cast<Type>(id)),
			dist(0.00001, MaxBallSpeed), connection(connection), addObjFunc(addFunc),
			removeObjFunc(removeFunc)
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
			return shielded;
		}

		bool IsCollideWith(std::shared_ptr<GameObject> other) override
		{
			if (IsShielded() && other->GetType() == Type::BallRed)
				return false;
			return true;
		}

		void CollideAction(std::shared_ptr<GameObject>)override {}

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
			default:
				break;
			}
			addObjFunc(wall);
		}

		void DoScript() override {
			auto result = std::chrono::high_resolution_clock::now() - lastShield;
			if (result > shieldTime && shielded == true)
				shielded = false;

			auto actions = connection->GetActions();
			if (actions.size() > 0)
			{
				for (auto el : actions)
				{
					switch (el.moveType)
					{
					case Internet::Shield:
						std::cout << "Player: " << GetType() << " IsShielded: " << IsShielded() << " Shield \n";
						MakeShield();
						break;
					case Internet::Shoot:
						std::cout << "Player: " << GetType() << " Shoot \n";
						MakeShoot();
						break;
					case Internet::StartMoveLeft:
						std::cout << "Player: " << GetType() << " MoveLeft \n";
						MoveLeft();
						break;
					case Internet::StartMoveRight:
						std::cout << "Player: " << GetType() << " MoveRight \n";
						MoveRight();
						break;
					case Internet::StopMoveLeft:
						std::cout << "Player: " << GetType() << " StopLeft \n";
						StopLeft();
						break;
					case Internet::StopMoveRight:
						std::cout << "Player: " << GetType() << " StopRight \n";
						StopRight();
						break;
					}
				}
			}
		}

	protected:
		void MakeShield()
		{
			auto result = std::chrono::high_resolution_clock::now() - lastShield;
			if (result > shieldCooldown)
			{
				lastShield = std::chrono::high_resolution_clock::now();
				shielded = true;
			}
		}

		void MakeShoot()
		{
			auto result = std::chrono::high_resolution_clock::now() - lastShoot;
			if (result > shootCooldown)
			{
				std::cout << "Try shoot\n";
				lastShoot = std::chrono::high_resolution_clock::now();

				auto ball = PrepareRedBall();

				addObjFunc(ball);
				std::cout << "Shooted\n";
			}
		}

		std::shared_ptr<BadBall> PrepareRedBall()
		{
			GameEngine::Pointf pos, velocity;
			PreparePosAndVelocityForRedBall(pos, velocity);
			auto ball = std::make_shared<BadBall>(pos, removeObjFunc);
			ball->SetPhysic({ velocity, sqrtf(velocity.x*velocity.x + velocity.y*velocity.y) });
			return ball;
		}

		void PreparePosAndVelocityForRedBall(GameEngine::Pointf& pos, GameEngine::Pointf& velocity)
		{
			if (horizontalMove)
			{
				pos = GetPos();
				pos.y -= 2 * size.y;
				pos.x += size.x / 2;
				velocity.x = MaxBallSpeed/2 - dist(random);
				velocity.y = -dist(random);
			}
			else
			{
				pos = GetPos();
				pos.y += size.y / 2;
				pos.x -= size.x * 2 * -direction;
				velocity.x = dist(random)*direction;
				velocity.y = MaxBallSpeed/2 - dist(random);
			}
		}

		void MoveLeft()
		{
			if (horizontalMove)
				SetPhysic({ { -moveSpeed*direction,0 },0 });
			else SetPhysic({ { 0, -moveSpeed*direction },0 });
		}

		void MoveRight()
		{
			if (horizontalMove)
				SetPhysic({ { moveSpeed*direction,0 },0 });
			else SetPhysic({ { 0, moveSpeed*direction },0 });
		}

		void StopLeft()
		{
			StopRight();
		}

		void StopRight()
		{
			SetPhysic({ {0,0},0 });
		}
	};
}

