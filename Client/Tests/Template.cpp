/*Checking SFML
*/
#pragma once
#include <SFML\Graphics.hpp>
#include "Tests.h"

bool TestNazwa();
AutoAdd Nazwa(&TestNazwa, "NazwaModulu", "NazwaTestu", true/*czy uruchomi� ten test*/);

bool TestNazwa()
{
	
	//Tutaj kod, kt�ry generuje jakie� wizualne co�

	return PromptMessage("Pytanie, czy wida� efekt. Jesli wida� na pytanie odpowiedzie� \"TAK\"");
}