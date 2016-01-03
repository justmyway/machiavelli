#pragma once
#include "Card.h"
class MagiersCard :
	public Card
{
public:
	MagiersCard();

	static std::unique_ptr<Card> __stdcall Create();

	~MagiersCard();
};

