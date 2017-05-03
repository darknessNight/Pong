#include "stdafx.h"
#include "GameWindow.h"
#include <Windows.h>
#include <iostream>

int main(int argc, char** argv)
{

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::string adres = "127.0.0.1";
	int port = 43562;
	/*std::cout << "Podaj adres:\n";
	std::cin >> adres;
	std::cout << "Podaj port:\n";
	std::cin >> port;*/
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	if (argc < 2) {
		std::thread th1([&]()
		{
			std::string tmp = argv[0];
			tmp += " 32";
			system(tmp.c_str());
		});

		std::thread th2([&]()
		{
			std::string tmp = argv[0];
			tmp += " 32";
			system(tmp.c_str());
		});

		GameWindow game(adres, port);
		game.start();

		if (th1.joinable())
			th1.join();
		if (th2.joinable())
			th2.join();
	}else
	{
		GameWindow game(adres, port);
		game.start();
	}

	return 0;
}