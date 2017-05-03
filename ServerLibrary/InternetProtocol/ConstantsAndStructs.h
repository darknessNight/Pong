#pragma once
#include <chrono>

namespace Pong
{
	namespace Consts
	{
		const double PLAYER_WIDTH = 0.1;
		const double BALL_RADIUS = 0.01;
		const double WALL_WIDTH = 1.f;
	}

	namespace Internet
	{
		enum UserIds
		{
			Player1 = 0xff0,
			Player2,
			Player3
		};

		struct ConnectionObject
		{
			enum Type
			{
				DeadWall,
				BallCommon,
				BallRed,
				Wall,
				Player1 = UserIds::Player1,
				Player2 = UserIds::Player2,
				Player3 = UserIds::Player3,
			};

			float x, y;
			Type type;
			unsigned char lives;
			bool shielded;
      
			bool operator==(const ConnectionObject& other) const
			{
				return x == other.x && y == other.y &&type == other.type&&lives == other.lives && shielded==other.shielded;
			}
		};

		enum UserActionTypes
		{
			StartMoveLeft,
			StopMoveLeft,
			StartMoveRight,
			StopMoveRight,
			Shield,
			Shoot
		};

		struct UserMove
		{
			UserActionTypes moveType;
			std::chrono::microseconds time;

			bool operator==(const UserMove& other) const
			{
				return moveType == other.moveType && time == other.time;
			}
		};
	}
}
