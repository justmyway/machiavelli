#include "LaboratoriumCard.h"

LaboratoriumCard::LaboratoriumCard()
{
}

std::unique_ptr<Card> LaboratoriumCard::Create()
{
	std::unique_ptr<Card> card(new LaboratoriumCard());
	return std::move(card);
}


LaboratoriumCard::~LaboratoriumCard()
{
}
