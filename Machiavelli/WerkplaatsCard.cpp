#include "WerkplaatsCard.h"

WerkplaatsCard::WerkplaatsCard()
{
}

std::unique_ptr<Card> WerkplaatsCard::Create()
{
	std::unique_ptr<Card> card(new WerkplaatsCard());
	return std::move(card);
}


WerkplaatsCard::~WerkplaatsCard()
{
}
