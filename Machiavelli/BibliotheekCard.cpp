#include "BibliotheekCard.h"

BibliotheekCard::BibliotheekCard()
{
}

std::unique_ptr<Card> BibliotheekCard::Create()
{
	std::unique_ptr<Card> card(new BibliotheekCard());
	return std::move(card);
}


BibliotheekCard::~BibliotheekCard()
{
}
