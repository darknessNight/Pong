#pragma once

#include "SFML\Graphics.hpp"
#include "../ServerLibrary/InternetProtocol/ConstantsAndStructs.h"
#include <vector>

#define WINDOWSIZE 500

class GameWindow {
public:
	void start();
	void startDrawing();
private:
	sf::RenderWindow window;
	bool stopDrawing;
};