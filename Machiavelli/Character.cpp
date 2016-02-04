#include "Character.h"

void Character::RoundReset()
{
	ownedBy = nullptr;
	dead = false;
	actionPreformed = false;
	robber = nullptr;
}

Character::~Character()
{
}
