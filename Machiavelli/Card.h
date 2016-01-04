#pragma once

#include <string>
#include <memory>
#include <map>

enum class CardColor {
	NONE,
	GEEL,
	BLAUW,
	GROEN,
	ROOD,
	LILA,
	ALL
};



class Card
{
public:
	Card();

	void setProperties(const std::string &cardName, const int cardCost, const std::string &cardColor);
	void setProperties(const std::string &cardName, const int cardCost, const std::string &cardColor, const std::string &cardDescription);

	std::string &Name() { return name; };
	int Cost() { return cost; };
	CardColor Color() { return color; };
	std::string &Description() { return description; };
	int Points() { return cost; };

	~Card();
private:
	std::string name;
	int cost;
	CardColor color;
	std::string description;

	const std::map<std::string, CardColor> card_color_map = {
		{ "geel", CardColor::GEEL },
		{ "blauw", CardColor::BLAUW },
		{ "groen", CardColor::GROEN },
		{ "rood", CardColor::ROOD },
		{ "lila", CardColor::LILA },
	};
};

