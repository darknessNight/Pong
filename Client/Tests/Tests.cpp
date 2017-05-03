#include "Tests.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>


static std::vector<bool(*)()>* funcs;
static std::vector<std::string>* cName;
static std::vector<std::string>* sName;
static unsigned* lastF;

void AddToList(bool(*f)(), std::string suitName, std::string caseName)
{
	if (funcs == nullptr) funcs = new std::vector<bool(*)()>;
	funcs->push_back(f);
	if (cName == nullptr) cName = new std::vector<std::string>;
	cName->push_back(caseName);
	if (sName == nullptr) sName = new std::vector<std::string>;
	sName->push_back(suitName);
}


std::string TestAll()
{
	//funcs.push_back(Test10);
	std::vector<bool(*)()> &funcs = *::funcs;

	std::stringstream ret;
	for ( int i =  0; i < funcs.size(); i++ )
	{
		ret << "\n DeadWall " << (*cName)[i] << ":" << (*sName)[i] << " result: ";
		ret << (funcs[i]()?"SUCCESS":"FAILED");
	}
	return ret.str();
}

std::string Test() {
	//funcs.push_back(Test10);
	std::vector<bool(*)()> &funcs = *::funcs;

	std::stringstream ret;
	if (lastF != nullptr) {
		int i = *lastF;
		ret << "\n DeadWall " << (*cName)[i] << ":" << (*sName)[i] << " result: ";
		ret << (funcs[i]() ? "SUCCESS" : "FAILED");
	}
	return ret.str();
}

AutoAdd::AutoAdd(bool(*f)(), std::string suitName, std::string caseName, bool last)
{
	AddToList(f, suitName, caseName);
	if (last) {
		lastF = new unsigned;
		(*lastF) = funcs->size() - 1;
	}
}

bool PromptMessage(std::string str)
{
	return MessageBoxA(nullptr, str.c_str(), "DeadWall", MB_YESNO | MB_ICONQUESTION)==IDYES;
}