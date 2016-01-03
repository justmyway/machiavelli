#include "KerkhofCard.h"

KerkhofCard::KerkhofCard()
{
}

std::unique_ptr<Card> KerkhofCard::Create()
{
	std::unique_ptr<Card> card(new KerkhofCard());
	return std::move(card);
}


KerkhofCard::~KerkhofCard()
{
}
