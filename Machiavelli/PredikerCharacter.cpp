#include "stdafx.h"
#include "PredikerCharacter.h"
#include "Player.h" 

std::shared_ptr<Character> PredikerCharacter::Create(const std::string name, int order)
{
	std::shared_ptr<Character> character(new PredikerCharacter(name, order));
	return std::move(character);
}

int PredikerCharacter::CollectCash()
{
	if (ownedBy) {
		return ownedBy->BuildingsWithColor(Color());
	}
}

PredikerCharacter::~PredikerCharacter()
{
}
