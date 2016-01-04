#pragma once
#include "Card.h"
class HofDerWonderenCard :
	public Card
{
public:
	HofDerWonderenCard();

	static std::unique_ptr<Card> __stdcall Create();

	CardColor Color() { return CardColor::ALL; };

	~HofDerWonderenCard();
};

