#pragma once
#include <string>
void AddToList(bool(*f)(),std::string suitName, std::string caseName);

class AutoAdd {
public:
	AutoAdd(bool(*f)(), std::string suitName, std::string caseName, bool last = false);
};

std::string Test();
std::string TestAll();

bool PromptMessage(std::string str);