#include "Character.h"

void Character::RoundReset()
{
	ownedBy = nullptr;
	dead = false;
	actionPreformed = false;
}

Character::~Character()
{
}
