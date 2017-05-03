/*Checking SFML
*/
#pragma once
#include <SFML\Graphics.hpp>
#include "Tests.h"

bool TestNazwa();
AutoAdd Nazwa(&TestNazwa, "NazwaModulu", "NazwaTestu", true/*czy uruchomiæ ten test*/);

bool TestNazwa()
{
	
	//Tutaj kod, który generuje jakieœ wizualne coœ

	return PromptMessage("Pytanie, czy widaæ efekt. Jesli widaæ na pytanie odpowiedzieæ \"TAK\"");
}