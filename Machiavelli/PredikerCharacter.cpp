#include "stdafx.h"
#include "PredikerCharacter.h"

std::unique_ptr<Character> PredikerCharacter::Create(const std::string name, int order)
{
	std::unique_ptr<Character> character(new PredikerCharacter(name, order));
	return std::move(character);
}

Character::CharacterColor PredikerCharacter::Color()
{
	return BLAUW;
}


PredikerCharacter::~PredikerCharacter()
{
}
