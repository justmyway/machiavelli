#pragma once

#include <string>
#include <memory>

enum class CharacterColor {
	NONE,
	GEEL,
	BLAUW,
	GROEN,
	ROOD,
};

class Game;
class Player;

class Character
{
public:

	//Setup
	Character(const std::string name, int order) : name{ name }, order{ order } {};

	const std::string Name() { return name; };
	const int Order() { return order; };
	CharacterColor Color() { return CharacterColor::NONE; };

	//Round
	void RoundReset();

	std::shared_ptr<Player> HoldedBy() { return ownedBy; };
	void HoldedBy(std::shared_ptr<Player> player) { ownedBy = player; };

	bool Killed() { return dead; };
	void kill() { dead = true; }

	std::shared_ptr<Player> Robed() { return robber; };
	void Rob(std::shared_ptr<Player> player) { robber = player; };

	bool ActionPreformed() { return actionPreformed; };
	void ActionPreformed(bool action) { actionPreformed = action; };

	int CollectCash() { return 0; };
	int maxToBuildBuildings(int amount) { return amount; };

	~Character();
private:
	const std::string name;
	const int order;

	//Round
	bool dead;
protected:
	//Round
	bool actionPreformed;
	std::shared_ptr<Player> ownedBy;
	std::shared_ptr<Player> robber;
};

