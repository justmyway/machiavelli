#pragma once
#include "Card.h"
class WerkplaatsCard :
	public Card
{
public:
	WerkplaatsCard();

	static std::unique_ptr<Card> __stdcall Create();

	~WerkplaatsCard();
};

