#pragma once
#include "Card.h"
class LaboratoriumCard :
	public Card
{
public:
	LaboratoriumCard();

	static std::unique_ptr<Card> __stdcall Create();

	~LaboratoriumCard();
};

