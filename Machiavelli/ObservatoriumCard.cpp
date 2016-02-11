#include "ObservatoriumCard.h"

ObservatoriumCard::ObservatoriumCard()
{
}

std::unique_ptr<Card> ObservatoriumCard::Create()
{
	std::unique_ptr<Card> card(new ObservatoriumCard());
	return std::move(card);
}


ObservatoriumCard::~ObservatoriumCard()
{
}
