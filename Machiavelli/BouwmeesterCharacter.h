#pragma once
#include "Character.h"
class BouwmeesterCharacter :
	public Character
{
public:
	BouwmeesterCharacter(const std::string name, int order) : Character{ name, order } {};
	static std::shared_ptr<Character> __stdcall Create(const std::string name, int order);

	int CardAmount(int amount);
	int maxToBuildBuildings(int amount) { return amount + 2; };

	~BouwmeesterCharacter();
};

