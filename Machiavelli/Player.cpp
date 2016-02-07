#include "Player.h"
#include "Socket.h"
#include "Game.h"

#include <sstream>

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

void Player::write(std::string value, bool start)
{
	*client << "\r" << value;
	if (start)
		*client << "\r\x1b[30;40mmachiavelli> ";
}

void Player::writeError(std::string value)
{
	write("\x1b[31;1m" + value + "\n");
}

int Player::CalculatePoints()
{
	return 0;
}

void Player::PrintOverview()
{
	std::shared_ptr<Player> opponent{ game->GetOpponent(client) };
	std::vector<std::string> opponent_info = opponent->PrintOverviewOpponent();
	//write("In het spel aanwezig:\n", false);
	write("==========================================================\n", false);
	for (auto &info : opponent_info)
		write(info, false);
	write("==========================================================\n", false);
	write("Jij, de machtige \"" + name + "\" hebt:\n", false);
	write("\x1b[33;1mGoud: " + std::to_string(stash) + "\x1b[30;40m\n", false);
	if (character_cards.size() > 0) {
		write("Karakterkaarten: \n", false);
		for (auto &caracter : character_cards)
			write(caracter->Name() + " ", false);

		write("\n", false);
	}
	write("Kaarten: " + std::to_string(cards.size()) + "\n", false);
	if (PrintCards().size() == 0) {
		write("   Jij hebt helaas geen kaarten meer.\n", false);
	}
	else {
		for (auto &info : PrintCards())
			write("   " + info + "\n", false);
	}
	write("Gebouwen: " + std::to_string(buildings.size()) + "\n", false);
	if (PrintBuildings().size() == 0) {
		write("   Jij hebt helaas nog geen gebouwen gebouwd.\n", false);
	}
	else {
		for (auto &info : PrintBuildings())
			write("   " + info + "\n", false);
	}
	write("==========================================================\n");
}

std::vector<std::string> Player::PrintOverviewOpponent()
{
	std::vector<std::string> info_string;
	info_string.push_back(name + " heeft: \n");
	info_string.push_back("\x1b[33;1mGoud: " + std::to_string(stash) + "\x1b[30;40m\n");
	info_string.push_back("Kaarten: " + std::to_string(cards.size()) + "\n");
	info_string.push_back("Gebouwen: " + std::to_string(buildings.size()) + "\n");
	std::vector<std::string> building_info = PrintBuildings();
	if (building_info.size() == 0) {
		info_string.push_back("   " + name + " heeft helaas nog geen gebouwen gebouwd.\n");
	}
	else {
		for (auto info : building_info)
			info_string.push_back("   " + info + "\n");
	}

	return info_string;
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

	for (auto const& i : cards) {
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
	fase = 1;
	ResetTurn();
	if (current_character->Robed() != nullptr) {
		write("\x1b[36;1mJe bent van " + std::to_string(stash) + " goud beroofd door " + current_character->Robed()->get_name() + ".\x1b[30;40m\n");
		current_character->Robed()->write("\x1b[36;1mJe hebt de " + current_character->Name() + " bestolen van " + std::to_string(stash) + " goud.\x1b[30;40m\n");
		current_character->Robed()->Deposit(stash);
		stash = 0;
	}
	write("Inkomsten fase, kies voor goud of kaarten.\n");
}

bool Player::CollectCash()
{
	if (fase != 1)
		return false;

	int collectGold = 2;

	collectGold += current_character->CollectCash();

	stash += collectGold;

	fase = 2;

	write("Je hebt " + std::to_string(collectGold) + " goud gekregen.\n");
	return true;
}

void Player::ChoseCards()
{
	int cardsToTake = 2;

	for (auto &building : buildings) {
		cardsToTake = building->CardAmount(cardsToTake);
	}

	while (cardsToTake > 0){
		drawn_cards.push_back(game->DrawCard());
		cardsToTake--;
	}

	write("Je kan een van de volgende kaarten kiezen:\n");

	int count = 0;

	for (auto const& i : drawn_cards) {
		write("   [" + std::to_string(count) + "] " + i->Name() + " (" + i->Color() + ", " + i->Cost() + ((i->Description().size() > 0) ? ", " + i->Description() : "") + ")\n");
		count++;
	};

	chosing_cards = true;
}

void Player::ChoseCards(std::string value)
{
	int cardsToTake = 1;
	for (auto &building : buildings) {
		cardsToTake = building->CardAmountKeep(cardsToTake);
	}

	if (cardsTaken < cardsToTake) {
		std::stringstream stream(value);
		std::string keeping_card;
		int keeping_card_index;
		stream >> keeping_card;
		keeping_card_index = std::stoi(keeping_card);

		if ((keeping_card_index < drawn_cards.size()) && (keeping_card_index >= 0)) {
			cards.push_back(std::move(drawn_cards.at(keeping_card_index)));
			drawn_cards.erase(drawn_cards.begin() + keeping_card_index);
			cardsTaken++;
			if (cardsTaken >= cardsToTake) {
				while (drawn_cards.size() > 0) {
					game->DiscardCard(std::move(drawn_cards.at(0)));
					drawn_cards.erase(drawn_cards.begin() + 0);
				}
				write("Je hebt de " + cards.back()->Name() + " gekozen.\n");

				int collectGold = current_character->CollectCash();

				stash += collectGold;

				if(collectGold != 0) write("Je hebt " + std::to_string(collectGold) + " goud gekregen van de gebouwen.\n");

				fase = 2;
				chosing_cards = false;
			}
			else {
				write("Je kan nog een van de volgende kaarten kiezen:\n");

				int count = 0;

				for (auto const& i : cards) {
					write("   [" + std::to_string(count) + "] " + i->Name() + " (" + i->Color() + ", " + i->Cost() + ((i->Description().size() > 0) ? ", " + i->Description() : "") + ")\n");
					count++;
				};
			}
		}
		else {
			writeError("Dit getal: " + std::to_string(keeping_card_index) + " bestaat niet\n");
		}
	}
	else {
		fase = 2;
		chosing_cards = false;
	}
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
	if (fase == 1) {
		writeError("Je zit momentaal in de inkomsten fase maak deze eerst af.");
		return false;
	}
	if (buildingsBuild < current_character->maxToBuildBuildings(1)) {
		if (buildingIndex < cards.size()) {
			int neededGold = cards.at(buildingIndex)->Cost();
			if (stash >= neededGold) {
				std::string buildingName = cards.at(buildingIndex)->Name();
				stash -= neededGold;
				buildings.push_back(std::move(cards.at(buildingIndex)));
				buildingsBuild++;
				if (buildings.size() >= 8)
					if (game->FirstFinishes())
						finish_points += 2;
				
				write("\"" + buildingName + "\" is gebouwd!\n.");
				return true;
			}
			else {
				writeError("Je hebt niet genoeg goud voor de \"" + cards.at(buildingIndex)->Name() + "\", je hebt nog " + std::to_string(neededGold - stash) + " goud nodig.\n");
			}
		}
		else {
			if (cards.size() == 0) {
				writeError("Deze kaart bestaat niet, Beschikbaar: 0-" + std::to_string(cards.size() - 1) + "\n");
			}
			else {
				writeError("Je hebt momentaal geen kaarten op hand, zorg dat je die eerst hebt.\n");
			}
		}
	}
	else {
		writeError("Je mag deze beurt niet meer gebouwen bouwen.\n");
	}
	return false;
}

void Player::Murder(std::string &name)
{
	if (current_character->Order() == 1) {
		if (!current_character->ActionPreformed()) {
			if (game->Murder(name)) {
				current_character->ActionPreformed(true);
			}
			else {
				writeError("Helaas maar \'" + name + "\' word niet herkend. \nProbeer het nog eens.");
			}
		}
		else {
			writeError("Je hebt deze ronde al iemand vermoord, eentje lijkt me al veel...");
		}
	}
	else {
		writeError("Je kan alleen iemand vermoorden als je de moordenaar bent!");
	}
}

void Player::Rob(std::string & name)
{
	if (current_character->Order() == 2) {
		if (!current_character->ActionPreformed()) {
			if (game->Rob(name)) {
				current_character->ActionPreformed(true);
			}
			else {
				writeError("Helaas maar \'" + name + "\' kan niet worden bestolen. \nProbeer het nog eens.");
			}
		}
		else {
			writeError("Je hebt deze ronde al iemand bestolen...");
		}
	}
	else {
		writeError("Je kan alleen iemand bestelen als je de dief bent!");
	}
}

void Player::SwapCards(std::string &name)
{
	if (current_character->Order() == 3) {
		if (!current_character->ActionPreformed()) {
			if (name.compare("tegenstander") == 0) {
				std::vector<std::unique_ptr<Card>> myCards = std::move(cards);
				cards.clear();
				std::vector<std::unique_ptr<Card>> thereCards = game->GetOpponent(client)->getCards();
				for (auto &card : thereCards) {
					cards.push_back(std::move(card));
				}
				for (auto &card : myCards) {
					game->GetOpponent(client)->addCard(std::move(card));
				}
				write("\x1b[36;1mJe hebt je kaarten met je tegenstander geruild!\x1b[30;40m\n");
				PrintOverview();
				game->GetOpponent(client)->write("\x1b[36;1mJe hebt je kaarten moeten ruilen met je tegenstander!\x1b[30;40m\n");
				game->GetOpponent(client)->PrintOverview();
				current_character->ActionPreformed(true);
			}
			else if (name.compare("voorraad") == 0) {
				int card_amount = cards.size();
				for (auto &card : cards)
					game->DiscardCard(std::move(card));
				
				cards.clear();
				for (int i = 0; i < card_amount; i++)
					cards.push_back(game->DrawCard());

				write("\x1b[36;1mJe hebt je kaarten met je voorraad geruild!\x1b[30;40m\n");
				PrintOverview();
				game->GetOpponent(client)->write("\x1b[36;1mJou tegenstander heeft zijn kaarten met de voorraad geruild!\x1b[30;40m\n");

				current_character->ActionPreformed(true);
			}
			else {
				writeError("Helaas maar \'" + name + "\' is geen optie(tegenstander of voorraad wel).");
			}
		}
		else {
			writeError("Je hebt deze ronde al kaarten geruild...");
		}
	}
	else {
		writeError("Je kan alleen kaarten ruilen als je de magier bent!");
	}
}

void Player::TakeTwoCards()
{
	if (current_character->Order() == 7) {
		if (!current_character->ActionPreformed()) {
			cards.push_back(game->DrawCard());
			cards.push_back(game->DrawCard());
			write("Twee extra kaarten zijn getrokken!\n");
			PrintOverview();
			game->GetOpponent(client)->write("\x1b[36;1mJou tegenstander heeft twee extra kaarten getrokken!\x1b[30;40m\n");
			current_character->ActionPreformed(true);
		}
		else {
			writeError("Je hebt deze ronde al kaarten gepakt...");
		}
	}
	else {
		writeError("Je kan alleen extra kaarten pakken als je bouwemeester bent!");
	}
}

void Player::Destroy(int index)
{
	if (current_character->Order() == 8) {
		if (!current_character->ActionPreformed()) {
			if (game->GetOpponent(client)->DestroyBuilding(index)) {
				current_character->ActionPreformed(true);
				write("Het gebouw is vernietigd!\n");
				PrintOverview();
				current_character->ActionPreformed(true);
			}
			else {
				write("Dit gebouw kon niet worden vernietigd!\n");
			}
		}
		else {
			writeError("Je hebt deze ronde een gebouw vernietigd...");
		}
	}
	else {
		writeError("Je kan alleen gebouwen vernietigen als je condottiere bent!");
	}
}

bool Player::DestroyBuilding(int index)
{
	if (index > 0 && index < buildings.size()) {
		for (auto &character : character_cards) {
			if (character->Order() == 5) {
				game->GetOpponent(client)->write("Je tegenstander is beschermd door de condottiere.\n");
				return false;
			}
		}

		if (buildings.at(index)->MayBeDestroyed()) {
			write("Het gebouw: " + buildings.at(index)->Name() + " is vernietigd door de condottiere.\n");
			buildings.erase(buildings.begin() + index);
			PrintOverview();
			return true;
		}
		else {
			game->GetOpponent(client)->write("Dit gebouw mag je niet vernietigen.\n");
		}
	}
	else {
		game->GetOpponent(client)->write("De gekozen index bestaat niet: " + std::to_string(index) + ".\n");
	}

	return false;
}

void Player::ResetTurn()
{
	buildingsBuild = 0;
	chosing_cards = false;
	cardsTaken = 0;
}
