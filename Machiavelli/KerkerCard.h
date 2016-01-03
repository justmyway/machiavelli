#pragma once
#include "Card.h"
class KerkerCard :
	public Card
{
public:
	KerkerCard();

	static std::unique_ptr<Card> __stdcall Create();

	~KerkerCard();
};

