#include "stdafx.h"
#include "KoningCharacter.h"

std::unique_ptr<Character> KoningCharacter::Create(const std::string name, int order)
{
	std::unique_ptr<Character> character(new KoningCharacter(name, order));
	return std::move(character);
}

Character::CharacterColor KoningCharacter::Color()
{
	return GEEL;
}


KoningCharacter::~KoningCharacter()
{
}
