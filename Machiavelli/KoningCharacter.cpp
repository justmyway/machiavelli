#include "stdafx.h"
#include "KoningCharacter.h"
#include "Player.h"

std::shared_ptr<Character> KoningCharacter::Create(const std::string name, int order)
{
	std::shared_ptr<Character> character(new KoningCharacter(name, order));
	return std::move(character);
}

int KoningCharacter::CollectCash()
{
	if(ownedBy){
		return ownedBy->BuildingsWithColor(Color());
	}
}

KoningCharacter::~KoningCharacter()
{
}
