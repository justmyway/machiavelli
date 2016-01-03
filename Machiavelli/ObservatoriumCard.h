#pragma once
#include "Card.h"
class ObservatoriumCard :
	public Card
{
public:
	ObservatoriumCard();

	static std::unique_ptr<Card> __stdcall Create();

	~ObservatoriumCard();
};

