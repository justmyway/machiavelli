#include "KoopmanCharacter.h"
#include "Player.h"

std::shared_ptr<Character> KoopmanCharacter::Create(const std::string name, int order)
{
	std::shared_ptr<Character> character(new KoopmanCharacter(name, order));
	return std::move(character);
}

int KoopmanCharacter::CollectCash()
{
	if (ownedBy) {
		actionPreformed = true;
		return ownedBy->BuildingsWithColor(Color()) + 1;
	}
	return 0;
}

KoopmanCharacter::~KoopmanCharacter()
{
}
