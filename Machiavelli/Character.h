#pragma once

#include <string>
#include <memory>

class Game;

class Character
{
public:

	enum CharacterColor {
		NONE,
		GEEL,
		BLAUW,
		GROEN,
		ROOD,
	};

	Character(const std::string name, int order) : name{ name }, order{ order } {};

	const std::string Name() { return name; };
	const int Order() { return order; };
	CharacterColor Color() { return NONE; };

	//Round
	int maxToBuildBuildings(int amount) { return amount; };

	~Character();
private:
	const std::string name;
	const int order;
};

