#pragma once

#include "SFML\Graphics.hpp"
#include "../ServerLibrary/InternetProtocol/ConstantsAndStructs.h"
#include <vector>

#define WINDOWSIZE 500

class GameWindow {
public:
	GameWindow(std::string adres, int port);
	void start();
	void startDrawing();
private:
	sf::RenderWindow window;
	bool stopDrawing;
	std::string adres;
	int port;
};