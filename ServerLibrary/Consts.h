#pragma once


namespace Pong
{
	const float PlayerMoveSpeed = 0.2f;
	const std::chrono::milliseconds PlayerShieldCooldown = std::chrono::milliseconds(30000);
	const std::chrono::milliseconds PlayerShootCooldown = std::chrono::milliseconds(10000);
	const std::chrono::milliseconds PlayerShieldTime = std::chrono::milliseconds(10000);
	const int PlayerInitLives = 3;
	const float MaxBallSpeed = 1.f;
	const float MinBallSpeed = 0.2f;
	const float BoardWidth = 1.f;
	const float BoardHeight = 1.f;
	const int BadBallLives = 10;
}