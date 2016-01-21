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
	return lhs += std::to_string(rhs);
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

void Player::NewRound()
{
	character_cards.clear();
}

void Player::CallCaracter(std::shared_ptr<Character> character)
{
	current_character = character;
	ResetTurn();
}

void Player::Fase1()
{
}

void Player::CollectCash()
{
	int collectGold = 2;

	collectGold += current_character->CollectCash();

	stash += collectGold;

	write("You got " + std::to_string(collectGold) + " gold");
}

void Player::ChoseCards()
{
	int cardsToTake = 2;

	for (auto &building : buildings) {
		cardsToTake = building->CardAmount(cardsToTake);
	}

	cardsToTake = current_character->CardAmount(cardsToTake);

	for (int i = 0; i < cardsToTake; i++)
		//drawn_cards.push_back(game->DrawCard());
		;

	//todo show drawn cards and display them
}

int Player::BuildingsWithColor(CharacterColor color)
{
	int NumberOfColorBuildings = 0;
	for (auto &building : buildings) {
		NumberOfColorBuildings += building->CollectCash(color);
	}
	return NumberOfColorBuildings;
}

bool Player::Build(unsigned int buildingIndex)
{
	if (buildingsBuild < current_character->maxToBuildBuildings(1)) {
		if (buildingIndex < cards.size()) {
			int neededGold = cards.at(buildingIndex)->Cost();
			if (stash >= neededGold) {
				std::string buildingName = cards.at(buildingIndex)->Name();
				stash -= neededGold;
				buildings.push_back(std::move(cards.at(buildingIndex)));
				buildingsBuild++;
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

void Player::Kill(std::string &name)
{
	
}

void Player::Destroy(int index)
{
}

void Player::ResetTurn()
{
	buildingsBuild = 0;
}

void Player::addCharacter(std::shared_ptr<Character> Character)
{
	character_cards.push_back(Character);
}
