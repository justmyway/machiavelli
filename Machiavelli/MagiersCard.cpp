#include "MagiersCard.h"
#include "Character.h"

MagiersCard::MagiersCard()
{
}

std::unique_ptr<Card> MagiersCard::Create()
{
	std::unique_ptr<Card> card(new MagiersCard());
	return std::move(card);
}

int MagiersCard::CollectCash(CharacterColor characterColor)
{
	return 1;
}


MagiersCard::~MagiersCard()
{
}
