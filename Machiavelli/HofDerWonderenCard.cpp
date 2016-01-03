#include "HofDerWonderenCard.h"

HofDerWonderenCard::HofDerWonderenCard()
{
}

std::unique_ptr<Card> HofDerWonderenCard::Create()
{
	std::unique_ptr<Card> card(new HofDerWonderenCard());
	return std::move(card);
}


HofDerWonderenCard::~HofDerWonderenCard()
{
}
