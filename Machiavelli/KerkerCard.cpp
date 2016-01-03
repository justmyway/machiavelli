#include "KerkerCard.h"

KerkerCard::KerkerCard()
{
}

std::unique_ptr<Card> KerkerCard::Create()
{
	std::unique_ptr<Card> card(new KerkerCard());
	return std::move(card);
}


KerkerCard::~KerkerCard()
{
}
