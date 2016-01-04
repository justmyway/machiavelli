#pragma once
#include "Card.h"
class PrestigieusGebouwCard :
	public Card
{
public:
	PrestigieusGebouwCard();

	static std::unique_ptr<Card> __stdcall Create();
	int Points() { return 8; };

	~PrestigieusGebouwCard();
};

