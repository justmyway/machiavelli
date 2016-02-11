#include "BouwmeesterCharacter.h"


std::shared_ptr<Character> BouwmeesterCharacter::Create(const std::string name, int order)
{
	std::shared_ptr<Character> character(new BouwmeesterCharacter(name, order));
	return std::move(character);
}

BouwmeesterCharacter::~BouwmeesterCharacter()
{
}
