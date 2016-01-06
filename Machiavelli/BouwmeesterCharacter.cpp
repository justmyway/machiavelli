#include "BouwmeesterCharacter.h"


std::unique_ptr<Character> BouwmeesterCharacter::Create(const std::string name, int order)
{
	std::unique_ptr<Character> character(new BouwmeesterCharacter(name, order));
	return std::move(character);
}


BouwmeesterCharacter::~BouwmeesterCharacter()
{
}
