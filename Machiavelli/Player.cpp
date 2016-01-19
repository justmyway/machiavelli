#include "Player.h"
#include "Socket.h"

using namespace std;

std::string& operator+(std::string & lhs, const CardColor &rhs) {
	std::map<CardColor, std::string> m;
	m[CardColor::GEEL] = "Geel";
	m[CardColor::BLAUW] = "Blauw";
	m[CardColor::GROEN] = "Groen";
	m[CardColor::ROOD] = "Rood";
	m[CardColor::LILA] = "Lila";
	m[CardColor::ALL] = "Lila";

	auto it = m.find(rhs);
	if (it != m.end())
		return lhs += it->second;

	return lhs += "Unknown";
};

std::string& operator+(std::string & lhs, const int rhs) {
	return lhs += rhs;
};

void Player::write(std::string value)
{
	*client << "\r" << value << "\rmachiavelli> ";
}

void Player::writeError(std::string value)
{
	write("\033[1; 31m" + value + "\033[0m\n");
}

std::vector<std::string> Player::PrintBuildings()
{
	std::vector<std::string> BuildingInfo;

	for (auto const& i : buildings) {
		BuildingInfo.push_back(i->Name() + " (" + i->Color() + ", " + i->Cost() + ((i->Description().size() > 0) ? ", " + i->Description() : "" ) + "):");
	};

	return BuildingInfo;
}

std::vector<std::string> Player::PrintCards()
{
	std::vector<std::string> CardInfo;

	for (auto const& i : buildings) {
		CardInfo.push_back(i->Name() + " (" + i->Color() + ", " + i->Cost() + ((i->Description().size() > 0) ? ", " + i->Description() : "") + "):");
	};

	return CardInfo;
}

bool Player::NewRound(int order)
{
	for (auto &character : character_cards)
	{
		if (character->Order() == order) {
			//set current player
			current_character = move(character);

			//round variables
			maxBuildingsToBuild = current_character->maxToBuildBuildings(1);
			return true;
		}
	}

	return false;
}

void Player::Fase1()
{
}

bool Player::Build(unsigned int buildingIndex)
{
	if (maxBuildingsToBuild != 0) {
		if (buildingIndex < cards.size()) {
			int neededGold = cards.at(buildingIndex)->Cost();
			if (stash >= neededGold) {
				std::string buildingName = cards.at(buildingIndex)->Name();
				stash -= neededGold;
				buildings.push_back(std::move(cards.at(buildingIndex)));
				maxBuildingsToBuild--;
				write("\"" + buildingName + "\" has been build.");
				return true;
			}
			else {
				writeError("You haven't got enough money for the \"" + cards.at(buildingIndex)->Name() + "\ to build, you need " + std::to_string(neededGold - stash) + " more gold.");
			}
		}
		else {
			if (cards.size() == 0) {
				writeError("Deze kaart bestaat niet, Beschikbaar: 0-" + std::to_string(cards.size() - 1));
			}
			else {
				writeError("Je hebt momentaal geen kaarten op hand, zorg dat je die eerst hebt.");
			}
		}
	}
	else {
		writeError("Je mag deze beurt niet meer gebouwen bouwen.");
	}
	return false;
}

void Player::addCharacter(std::unique_ptr<Character>& Character)
{
	character_cards.push_back(Character);
}
