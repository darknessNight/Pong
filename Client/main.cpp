#define TEST_ALL //sprawia, ¿e uruchamia wszystkie testy, a nie tylko ten oznaczony
#include "stdafx.h"
#include "Tests\Tests.h"
#include <iostream>
#define TESTS_START 0
#define TESTS_STOP INT_MAX

int main()
{
#ifdef TEST_ALL
	std::string output = TestAll();
#else
	std::string output = Test();
#endif
	std::clog << output << "\n\n\n" << std::endl;
	system("pause");
	return 0;
}