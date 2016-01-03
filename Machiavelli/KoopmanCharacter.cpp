#include "stdafx.h"
#include "KoopmanCharacter.h"

std::unique_ptr<Character> KoopmanCharacter::Create(const std::string name, int order)
{
	std::unique_ptr<Character> character(new KoopmanCharacter(name, order));
	return std::move(character);
}

Character::CharacterColor KoopmanCharacter::Color()
{
	return GROEN;
}


KoopmanCharacter::~KoopmanCharacter()
{
}
