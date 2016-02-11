#pragma once
#include "Card.h"
class KerkhofCard :
	public Card
{
public:
	KerkhofCard();

	static std::unique_ptr<Card> __stdcall Create();

	~KerkhofCard();
};

