#include "stdafx.h"
#include "GameWindow.h"
#include <Windows.h>
#include <iostream>

int main()
{
	std::string adres;
	int port;
	std::cout << "Podaj adres:\n";
	std::cin >> adres;
	std::cout << "Podaj port:\n";
	std::cin >> port;
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	GameWindow window(adres,port);
	window.start();
	
	return 0;
}