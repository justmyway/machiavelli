#include "Card.h"

Card::Card()
{
}

void Card::setProperties(const std::string &cardName, const int cardCost, const std::string & cardColor)
{
	name = cardName;
	cost = cardCost;
	
	auto it = card_color_map.find(cardColor);
	if (it == card_color_map.end())
		throw std::runtime_error("Color " + cardColor + " could not be found! \n @initialising cards. \n'");

	color = it->second;
}

void Card::setProperties(const std::string & cardName, const int cardCost, const std::string & cardColor, const std::string & cardDescription)
{
	setProperties(cardName, cardCost, cardColor);
	description = cardDescription;
}


Card::~Card()
{
}
