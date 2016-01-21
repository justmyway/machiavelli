#include "BouwmeesterCharacter.h"


std::shared_ptr<Character> BouwmeesterCharacter::Create(const std::string name, int order)
{
	std::shared_ptr<Character> character(new BouwmeesterCharacter(name, order));
	return std::move(character);
}

int BouwmeesterCharacter::CardAmount(int amount)
{
	actionPreformed = true;
	return amount + 2;
}


BouwmeesterCharacter::~BouwmeesterCharacter()
{
}
