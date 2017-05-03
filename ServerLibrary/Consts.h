#pragma once


namespace Pong
{
	const float PlayerMoveSpeed = 1.f;
	std::chrono::milliseconds PlayerShieldCooldown = std::chrono::milliseconds(30000);
	std::chrono::milliseconds PlayerShootCooldown = std::chrono::milliseconds(10000);
	std::chrono::milliseconds PlayerShieldTime = std::chrono::milliseconds(10000);
	int PlayerInitLives = 3;
	float MaxBallSpeed = 1.f;
	float BoardWidth = 100.f;
	float BoardHeight = 100.f;
	int BadBallLives = 10;
}