#pragma once

#include "SFML/Graphics.hpp"
#include "../ServerLibrary/InternetProtocol/JsonClientProtocolConnection.h"

#define WINDOWSIZE 500

class GameWindow {
public:
	GameWindow(std::string adres, int port);
	void start();
	void startDrawing();
private:
	bool IsWindowOpen();
	bool PoolEventIfCan(sf::Event &event);

	std::mutex mutex;
	std::shared_ptr<Pong::Internet::ClientProtocolConnection> connection;
	sf::RenderWindow window;
	bool stopDrawing;
	std::string adres;
	int port;
};