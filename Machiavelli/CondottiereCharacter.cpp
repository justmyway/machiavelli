#include "stdafx.h"
#include "CondottiereCharacter.h"

std::unique_ptr<Character> CondottiereCharacter::Create(const std::string name, int order)
{
	std::unique_ptr<Character> character(new CondottiereCharacter(name, order));
	return std::move(character);
}

Character::CharacterColor CondottiereCharacter::Color()
{
	return ROOD;
}


CondottiereCharacter::~CondottiereCharacter()
{
}
