#pragma once

namespace Pong
{
	namespace Consts
	{
		const int PLAYER_WIDTH = 0.1;
		const int BALL_RADIUS = 0.01;
		const int WALL_WIDTH = 1.f;
	}

	namespace Internet
	{
		enum UserIds
		{
			Player1=0xff0,
			Player2,
			Player3
		};

		struct ConnectionObject
		{
			enum Type
			{
				Test,
				BallCommon,
				BallRed,
				Wall,
				Player1=0xff0,
				Player2,
				Player3,
			};

			float x, y;
			Type type;
		};

		enum UserActionTypes
		{

		};
	}
}