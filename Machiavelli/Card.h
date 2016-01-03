#pragma once

#include <string>
#include <memory>
#include <map>

class Card
{
public:

	enum CardColor {
		NONE,
		GEEL,
		BLAUW,
		GROEN,
		ROOD,
		LILA,
		ALL
	};

	Card();

	void setProperties(const std::string &cardName, const int cardCost, const std::string &cardColor);
	void setProperties(const std::string &cardName, const int cardCost, const std::string &cardColor, const std::string &cardDescription);

	CardColor Color() { return color; };

	~Card();
private:
	std::string name;
	int cost;
	CardColor color;
	std::string description;

	const std::map<std::string, CardColor> card_color_map = {
		{ "geel", GEEL },
		{ "blauw", BLAUW },
		{ "groen", GROEN },
		{ "rood", ROOD },
		{ "lila", LILA },
	};
};

