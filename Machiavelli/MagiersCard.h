#pragma once
#include "Card.h"

class Character;

class MagiersCard :
	public Card
{
public:
	MagiersCard();

	static std::unique_ptr<Card> __stdcall Create();

	int CollectCash(CharacterColor characterColor);

	~MagiersCard();
};

