#include "Game.h"

#include "BouwmeesterCharacter.h"
#include "StandardCharacter.h"
#include "CondottiereCharacter.h"
#include "KoningCharacter.h"
#include "KoopmanCharacter.h"
#include "PredikerCharacter.h"

#include "StandardCard.h"
#include "HofDerWonderenCard.h"
#include "KerkerCard.h"
#include "KerkhofCard.h"
#include "LaboratoriumCard.h"
#include "WerkplaatsCard.h"
#include "ObservatoriumCard.h"
#include "BibliotheekCard.h"
#include "MagiersCard.h"
#include "PrestigieusGebouwCard.h"

#include "ClientCommand.h"
#include "Socket.h"

#include <iostream>
#include <exception>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <windows.h>
#include <chrono>
#include <random>

Game::Game() :
	file_reader(new FileReader())
{
	//commands
	valid_commands = {
		"overzicht",
		"innen",
		"kaart",
		"kies",
		"bouw",
		"vermoord",
		"besteel",
		"ruil",
		"vernietig",
		"volgende",
		"chat"
	};

	//fill character map
	character_map["Bouwmeester"] = BouwmeesterCharacter::Create("Bouwmeester", 7);
	character_map["Condottiere"] = CondottiereCharacter::Create("Condottiere", 8);
	character_map["Koning"] = KoningCharacter::Create("Koning", 4);
	character_map["Koopman"] = KoopmanCharacter::Create("Koopman", 6);
	character_map["Prediker"] = PredikerCharacter::Create("Prediker", 5);

	//fill deck map
	FillDeckMap();

	//get characters
	PrepareCharacters();

	//get cards
	PrepareCards();

	//game properties
	started = false;
	first_finishes = false;
	GoldReserve = 30;

	//shuffle cards
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	auto engine = std::default_random_engine{};
	std::shuffle(clean_deck.begin(), clean_deck.end(), engine);
	deck = std::queue<std::unique_ptr<Card>>(std::move(clean_deck));

	//cheatmode 
	cheat_mode = true;

	std::cerr << "new game created. \n";
}

void Game::FillDeckMap()
{
	deck_map["Hof der Wonderen"] = HofDerWonderenCard::Create();
	deck_map["Kerker"] = KerkerCard::Create();
	deck_map["Kerkhof"] = KerkhofCard::Create();
	deck_map["Laboratorium"] = LaboratoriumCard::Create();
	deck_map["Werkplaats"] = WerkplaatsCard::Create();
	deck_map["Observatorium"] = ObservatoriumCard::Create();
	deck_map["Bibliotheek"] = BibliotheekCard::Create();
	deck_map["School voor magiërs"] = MagiersCard::Create();
	deck_map["Drakenpoort"] = PrestigieusGebouwCard::Create();
	deck_map["Universiteit"] = PrestigieusGebouwCard::Create();
}

void Game::PrepareCharacters()
{
	//create all characters based on file
	std::vector<std::string> character_lines = file_reader->Read(character_path);

	for (auto &line : character_lines) {

		if (line == FileReader::_EOF)
			continue;

		std::string &name = file_reader->ColumnValue(line, 2);
		const int order = std::stoi(file_reader->ColumnValue(line, 1));

		auto it = character_map.find(name);
		if (it != character_map.end()) {
			std::shared_ptr<Character> character(std::move(it->second));
			characterset.push_back(std::move(character));
		}
		else
		{
			std::shared_ptr<Character> character(new StandardCharacter(name, order));
			characterset.push_back(std::move(character));
		}
	}

	//check
	/*
	for (auto const& i : characterset) {
	std::cout << i->Name() << " - " << i->Color() << std::endl;
	};
	std::cout << "All " << characterset.size() << " characters have been made. \n";
	*/
	//end check
}

void Game::PrepareCards()
{
	//create all cards based on file
	std::vector<std::string> card_lines = file_reader->Read(deck_path);

	try {
		for (auto &line : card_lines) {

			if (line == FileReader::_EOF)
				continue;

			std::string &name = file_reader->ColumnValue(line, 1);
			const int cost = std::stoi(file_reader->ColumnValue(line, 2));
			std::string &color = file_reader->ColumnValue(line, 3);

			auto it = deck_map.find(name);
			if (it != deck_map.end()) {
				if (it->second == nullptr) {
					FillDeckMap();
					it = deck_map.find(name);
				}
				std::unique_ptr<Card> card(std::move(it->second));
				card->setProperties(name, cost, color, file_reader->ColumnValue(line, 4));
				clean_deck.push_back(std::move(card));
			}
			else
			{
				std::unique_ptr<Card> card(new StandardCard());
				card->setProperties(name, cost, color);
				clean_deck.push_back(std::move(card));
			}
		}

	}
	catch (const std::exception& ex) {
		std::cerr << "***************************************\n";
		std::cerr << ex.what();
		std::cerr << "***************************************\n";
	}

	//check
	/*
	std::cout << "All " << deck.size() << " cards have been made. \n";
	*/
	//end check
}

bool Game::Particatate(std::shared_ptr<Player> player)
{
	if (players.size() > 1)
		throw std::runtime_error("Trying to put more than 2 Players in a game. \r\n '" + player->get_name() + "' can't join!");

	players.push_back(player);

	return players.size() == 2;
}

void Game::Start()
{
	std::cerr << "game started. \n";

	Write("\x1b[30;40mDe game gaat beginnen! \n");

	started = true;

	//setup start
	current_player = players.front();
	start_player = players.front();

	for (auto &player : players) {

		player->Ingame(std::shared_ptr<Game>(this));
		player->Deposit(2);
		GoldReserve -= 2;

		for (int i = 0; i < 4; i++) {
			player->addCard(std::move(deck.front()));
			deck.pop();
		}
	}

	//ready game
	if (cheat_mode)	Write("\x1b[35;40m>> CHEATMODE << De kaarten worden al voor je gekozen\n\x1b[30;40m");
	SetupRound();
	if (cheat_mode) {
		ChoseCards(nullptr, "0");
		ChoseCards(nullptr, "1 0");
		ChoseCards(nullptr, "0 1");
		ChoseCards(nullptr, "0 1");
	}
}

bool Game::Execute(std::shared_ptr<ClientCommand> command)
{
	//command by one of these players?
	if (std::find(players.begin(), players.end(), command->get_player()) == players.end())
		return false;

	std::stringstream stream(command->get_cmd());
	std::string cmd;
	stream >> cmd;

	//check valid commands
	if (std::find(valid_commands.begin(), valid_commands.end(), cmd) == valid_commands.end()) {
		InvalidCommand(command, "Dit commando word niet herkent!");
		return true;
	}

	//check chat
	if (cmd.compare("chat") == 0) {
		std::pair<std::string, std::string> line = std::make_pair(command->get_player()->get_name(), command->get_cmd().substr(cmd.size(), command->get_cmd().size()) + "           ");
		chat_history.push_back(line);
		WriteChatLine(line);
		return true;
	}

	if (!started) {
		InvalidCommand(command, "Tja... niet dat het spel al is begonnen of zo...");
		return true;
	}

	//When the cards are devided
	if (roundSetup) {
		if (cmd.compare("kies") == 0 && YourTurn(command)) {
			ChoseCards(command, command->get_cmd().substr(cmd.size(), command->get_cmd().size()));
		}
		else {
			command->get_player()->writeError("Dit commando word niet herkent.");
		}
		return true;
	}

	if (cmd.compare("kies") == 0 && YourTurn(command) && current_player->Busy()) {
		current_player->ChoseCards(command->get_cmd().substr(cmd.size(), command->get_cmd().size()));
		return true;
	}
	else if (current_player->Busy() && current_player == command->get_player()) {
		current_player->writeError("Dit is nu niet van toepassing!");
		current_player->writeError("Kies een kaart!");
		return true;
	}

	if (cmd.compare("overzicht") == 0) {
		command->get_player()->PrintOverview();
		return true;
	}

	if (!YourTurn(command)) {
		command->get_player()->writeError("Het is helaas niet jou beurt.");
		return true;
	}

	//is next player, call next character
	if (cmd.compare("volgende") == 0) {
		while (!CallNextCharacter())
			;
		return true;
	}

	if (cmd.compare("innen") == 0) {
		current_player->CollectCash();
		return true;
	}

	if (cmd.compare("kaart") == 0) {
		current_player->ChoseCards();
		return true;
	}

	if (cmd.compare("bouw") == 0) {
		current_player->Build(std::stoi(command->get_cmd().substr(cmd.size(), command->get_cmd().size())));
		return true;
	}

	if (cmd.compare("vermoord") == 0) {
		current_player->Murder(command->get_cmd().substr(cmd.size()+1, command->get_cmd().size()));
		return true;
	}

	if (cmd.compare("besteel") == 0) {
		current_player->Rob(command->get_cmd().substr(cmd.size()+1, command->get_cmd().size()));
		return true;
	}

	if (cmd.compare("ruil") == 0) {
		current_player->SwapCards(command->get_cmd().substr(cmd.size()+1, command->get_cmd().size()));
		return true;
	}

	if (cmd.compare("kaarten") == 0) {
		current_player->TakeTwoCards();
		return true;
	}

	if (cmd.compare("vernietig") == 0) {
		std::stringstream stream(cmd);
		std::string destroy_card;
		int destroy_building_index;
		stream >> destroy_card;
		destroy_building_index = std::stoi(destroy_card);

		current_player->Destroy(destroy_building_index);
		return true;
	}

	return true;
}

Game::~Game()
{
}

void Game::ClearScreen(std::shared_ptr<Player> player)
{
	player->write("\33[2J\x1b[30;40m");
}

void Game::InvalidCommand(std::shared_ptr<ClientCommand> command, std::string value)
{
	std::shared_ptr<Socket> out = command->get_client();

	out->write("\33[31;40m" + value + "\r\n\x1b[30;40mMachiavelli> ");
}

void Game::Write(const std::string &value)
{
	for (auto &i : players) {
		i->write(value);
	}
}

void Game::WriteChat()
{
	for (std::pair<std::string, std::string> &line : chat_history) {
		WriteChatLine(line);
	}
}

void Game::WriteChatLine(const std::pair<std::string, std::string> &line)
{
	std::string finished_line = "\x1b[35;1m" + line.first + " >> " + line.second + "\x1b[37;40m\r\n";

	for (auto &i : players) {
		i->write(finished_line);
	}
}

void Game::SetupRound()
{
	roundSetup = true;

	choseable_characterset.clear();
	discard_characterset.clear();

	for (auto &character : characterset) {
		if (character->Order() == 4) {
			if (character->HoldedBy() != nullptr)
				start_player = character->HoldedBy();
		}
		character->RoundReset();
		choseable_characterset.push_back(character);
	}

	auto engine = std::default_random_engine{};
	std::shuffle(choseable_characterset.begin(), choseable_characterset.end(), engine);

	order = 0;

	for (auto& player : players) {
		ClearScreen(player);
		player->PrintOverview();
	}
	current_player = start_player;
	RemoveFirstCard();
}

void Game::RemoveFirstCard()
{
	if (choseable_characterset.front()->Order() != 4) {
		current_player->write("De eerste kaart die afgelegd is: " + choseable_characterset.front()->Name() + "\n");
		current_player->write("Je kan kiezen uit de volgende kaarten: \n");
		discard_characterset.push_back(choseable_characterset.front());
		choseable_characterset.pop_front();
	}
	else {
		current_player->write("De eerste kaart die afgelegd is: " + choseable_characterset.at(1)->Name() + "\n");
		current_player->write("Je kan kiezen uit de volgende kaarten: \n");
		discard_characterset.push_back(choseable_characterset.at(1));
		choseable_characterset.erase(choseable_characterset.begin() + 1);
	}

	PrintChosableCaracters();
}

void Game::ChoseCards(std::shared_ptr<ClientCommand> command, std::string cmd)
{
	std::stringstream stream(cmd);
	std::string keeping_card;
	int keeping_card_index;
	stream >> keeping_card;
	keeping_card_index = std::stoi(keeping_card);

	//first pick
	if (choseable_characterset.size() == 7) {
		if (keeping_card_index < choseable_characterset.size()) {
			PickCard(keeping_card_index);
			//next player
			NextPlayer();
			current_player->write("Je kan een van de volgende kaarten kiezen: \n");
			PrintChosableCaracters();
			return;
		}
		else {
			current_player->writeError("Dat number bestaat niet! Deze kaart kan je niet kiezen. \nProbeer het nogmaals.");
			return;
		}
	}
	else {
		//next rounds
		std::string losing_card;
		int losing_card_index;
		stream >> losing_card;
		losing_card_index = std::stoi(losing_card);

		if ((keeping_card_index < choseable_characterset.size()) && (losing_card_index < choseable_characterset.size()) && !(keeping_card_index == losing_card_index)) {

			if (keeping_card_index < losing_card_index)
				losing_card_index--;

			PickCard(keeping_card_index);
			RemoveCard(losing_card_index);
		}
		else {
			current_player->writeError("Dat number bestaat niet! Hoogste nummer is " + std::to_string(choseable_characterset.size()) + "\nProbeer het nogmaals.");
			return;
		}
	}

	//see if setup is done
	if (choseable_characterset.size() == 0) {
		roundSetup = false;
		while (!CallNextCharacter())
			;
	}
	else {
		//next player
		NextPlayer();
		current_player->write("Je kan een van de volgende kaarten kiezen en afleggen: \n");
		PrintChosableCaracters();
	}
}

void Game::PickCard(int index)
{
	current_player->write("Je hebt de \x1b[32;40m" + choseable_characterset.at(index)->Name() + "\x1b[30;40m gekozen. \n");
	choseable_characterset.at(index)->HoldedBy(current_player);
	current_player->addCharacter(choseable_characterset.at(index));
	choseable_characterset.erase(choseable_characterset.begin() + index);
}

void Game::RemoveCard(int index)
{
	current_player->write("Je hebt de \x1b[31;40m" + choseable_characterset.at(index)->Name() + "\x1b[30;40m weg gestopt. \n");
	discard_characterset.push_back(choseable_characterset.at(index));
	choseable_characterset.erase(choseable_characterset.begin() + index);
}

void Game::PrintChosableCaracters()
{
	for (int i = 0; i < choseable_characterset.size(); i++) {
		current_player->write("   [" + std::to_string(i) + "]  " + choseable_characterset.at(i)->Name() + " - prioriteit " + std::to_string(choseable_characterset.at(i)->Order()) + "\n");
	}
}

void Game::NextPlayer()
{
	if (players.at(0) == current_player) {
		current_player = players.at(1);
	}
	else {
		current_player = players.at(0);
	}
}

bool Game::YourTurn(std::shared_ptr<ClientCommand> command)
{
	if (command->get_player() == current_player) {
		return true;
	}
	else {
		command->get_player()->writeError("It is not your turn, wait a moment.");
		return false;
	}
}

std::shared_ptr<Player> Game::GetOpponent(std::shared_ptr<Socket> socket)
{
	if (players.at(0)->GetSocket() == socket)
		return players.at(1);
	
	return players.at(0);
}

bool Game::CallNextCharacter()
{
	order++;
	if (order == 9) {
		if (GameWon()) {
			current_player = nullptr;
			return true;
		}
		SetupRound();
		return true;
	}

	if (order == 1) Write("\x1b[33;1mDe Koning is: " + start_player->get_name() + "\x1b[30;40m\n");

	if (characterset.at(order - 1)->Killed()) {
		return false;
	}

	CallCharacter();

	current_player = characterset.at(order - 1)->HoldedBy();

	if (current_player != nullptr) {
		current_player->CallCaracter(characterset.at(order - 1));
		return true;
	}

	return false;
}

void Game::CallCharacter()
{
	Write("\x1b[33;1mKoning: \"Ik zou graag de " + characterset.at(order - 1)->Name() + " naar voren roepen!\"\x1b[30;40m\n");
}

bool Game::Murder(std::string & name)
{
	for (auto &character : characterset) {
		if (character->Order() != 1 && character->Name().compare(name) == 0) {
			character->kill();
			Write("\x1b[36;1mWat een drama en bende, de " + name + " is vermoord!\x1b[30;40m\n");
			return true;
		}
	}

	return false;
}

bool Game::Rob(std::string & name)
{
	std::cout << name << std::endl;
	for (auto &character : characterset) {
		std::cout << character->Name() << " " << character->Order() << " " << character->Killed() << std::endl;
		if (character->Order() > 2 && !character->Killed()) {
			if (character->Name().compare(name) == 0) {
				character->Rob(current_player);
				Write("\x1b[36;1m" + current_player->get_name() + " beroofd de " + name + "!\x1b[30;40m\n");
				return true;
			}
		}
	}

	return false;
}

std::unique_ptr<Card> Game::DrawCard()
{
	if (deck.size() == 0) {
		auto engine = std::default_random_engine{};
		std::shuffle(discard_deck.begin(), discard_deck.end(), engine);
		deck = std::queue<std::unique_ptr<Card>>(std::move(discard_deck));
	}
	auto item = std::move(deck.front());
	deck.pop();
	return std::move(item);
}

void Game::DiscardCard(std::unique_ptr<Card> card)
{
	discard_deck.push_back(std::move(card));
}

bool Game::GameWon()
{
	bool won = false;

	for (auto &player : players) {
		if (player->Buildings() >= 8) won = true;
	}

	//todo calculate points & show winner
	if (won) {
		std::pair<std::shared_ptr<Player>, int> winning_player = std::pair<std::shared_ptr<Player>, int>();

		for (auto &player : players) {
			if (winning_player.first == nullptr) {
				winning_player.first = player;
				winning_player.second = player->CalculatePoints();
				Write(player->get_name() + " heeft " + std::to_string(winning_player.second) + " punten gehaald! \n");
			}
			else {
				int points = player->CalculatePoints();
				if (points > winning_player.second) {
					winning_player.first = player;
					winning_player.second = points;
				}
				Write(player->get_name() + " heeft " + std::to_string(winning_player.second) + " punten gehaald! \n");
			}
		}

		Write("-----------------------------------------------------------\n");
		Write(winning_player.first->get_name() + " heeft gewonnen met " + std::to_string(winning_player.second) + " punten!!!!!!!!!!!!!!!! \n");
		Write("-----------------------------------------------------------\n");
	}

	return won;
}

bool Game::FirstFinishes()
{
	if (!first_finishes) {
		first_finishes = true;
		return true;
	}
	return false;
}
