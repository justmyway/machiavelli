#include "PrestigieusGebouwCard.h"

PrestigieusGebouwCard::PrestigieusGebouwCard()
{
}

std::unique_ptr<Card> PrestigieusGebouwCard::Create()
{
	std::unique_ptr<Card> card(new PrestigieusGebouwCard());
	return std::move(card);
}


PrestigieusGebouwCard::~PrestigieusGebouwCard()
{
}
