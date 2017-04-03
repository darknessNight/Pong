//#define TEST_ALL //sprawia, ¿e uruchamia wszystkie testy, a nie tylko ten oznaczony
#include "stdafx.h"
#include "GameWindow.h"
#include <Windows.h>
#include <iostream>
//#include "Tests\Tests.h"
//#include <iostream>
//#define TESTS_START 0
//#define TESTS_STOP INT_MAX

int main()
{
/*#ifdef TEST_ALL
	std::string output = TestAll();
#else
	std::string output = Test();
#endif
	std::clog << output << "\n\n\n" << std::endl;
	system("pause");*/
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