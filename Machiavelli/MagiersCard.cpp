#include "MagiersCard.h"

MagiersCard::MagiersCard()
{
}

std::unique_ptr<Card> MagiersCard::Create()
{
	std::unique_ptr<Card> card(new MagiersCard());
	return std::move(card);
}


MagiersCard::~MagiersCard()
{
}
