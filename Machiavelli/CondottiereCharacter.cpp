#include "CondottiereCharacter.h"
#include "Player.h"

std::shared_ptr<Character> CondottiereCharacter::Create(const std::string name, int order)
{
	std::shared_ptr<Character> character(new CondottiereCharacter(name, order));
	return std::move(character);
}

int CondottiereCharacter::CollectCash()
{
	if (ownedBy) {
		return ownedBy->BuildingsWithColor(Color());
	}
}

CondottiereCharacter::~CondottiereCharacter()
{
}
