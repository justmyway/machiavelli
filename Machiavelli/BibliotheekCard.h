#pragma once
#include "Card.h"

class BibliotheekCard :
	public Card
{
public:
	BibliotheekCard();

	static std::unique_ptr<Card> __stdcall Create();

	int CardAmountKeep(int amount) { return amount++; };

	~BibliotheekCard();
};

