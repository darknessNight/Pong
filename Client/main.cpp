#include "stdafx.h"
#include "GameWindow.h"
#include <Windows.h>
#include <iostream>

int main(int argc, char** argv)
{

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::string adres = "127.0.0.1";
	int port = 43562;
	std::cout << "Podaj adres:\n";
	std::cin >> adres;
	std::cout << "Podaj port:\n";
	std::cin >> port;

	GameWindow game(adres, port);
	game.start();

	return 0;
}
