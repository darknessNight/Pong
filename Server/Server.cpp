// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "ServerLibrary/Pong.h"

int ReadPort()
{
	int port = 43562;

	std::cout << "Uzyæ domyslnego portu ("<<port<<")?\n";
	char ans;
	std::cin >> ans;
	if(ans!='T' && ans !='t' && ans !='y' && ans!='Y')
	{
		std::cout << "Podaj port: ";
		std::cin >> port;
	}
	return port;
}

int main()
{
	int port = ReadPort();

	Pong::Pong game;

	game.StartServer(port);

    return 0;
}

