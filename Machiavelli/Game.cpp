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

Game::Game() :
	file_reader(new FileReader())
{
	//commands
	valid_commands = {
		"overzicht",
		"bouw",
		"vermoord",
		"besteel",
		"ruil",
		"vernietig",
		"next",
		"chat"
	};

	//fill character map
	character_map["Bouwmeester"]	= BouwmeesterCharacter::Create("Bouwmeester", 7);
	character_map["Condottiere"]	= CondottiereCharacter::Create("Condottiere", 8);
	character_map["Koning"]			= KoningCharacter::Create("Koning", 4);
	character_map["Koopman"]		= KoopmanCharacter::Create("Koopman", 6);
	character_map["Prediker"]		= PredikerCharacter::Create("Prediker", 5);

	//fill deck map
	FillDeckMap();

	//get characters
	PrepareCharacters();

	//get cards
	PrepareCards();

	//gold
	GoldReserve = 30;

	/*
	ALREADY BUILD

	Card.Points();
		returns the point for the card player->field->Card.Points()
	
	*/
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
			std::unique_ptr<Character> character(std::move(it->second));
			characterset.push_back(std::move(character));
		}
		else 
		{
			std::unique_ptr<Character> character(new StandardCharacter(name, order));
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
				deck.push(std::move(card));
			}
			else
			{
				std::unique_ptr<Card> card(new StandardCard());
				card->setProperties(name, cost, color);
				deck.push(std::move(card));
			}
		}

	}catch (const std::exception& ex) {
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

	for (auto &i : players) {
		ClearScreen(i);
		i->write("De game gaat beginnen! \r\n");
	}

	started = true;
}

bool Game::Execute(std::shared_ptr<ClientCommand> command)
{
	//command by one of these players?
	if(std::find(players.begin(), players.end(), command->get_player()) == players.end())
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
		std::pair<std::string, std::string> line = std::make_pair(command->get_player()->get_name(), command->get_cmd().substr(cmd.size(), command->get_cmd().size()));
		chat_history.push_back(line);
		WriteChatLine(line);
		return true;
	}

	if (!started) {
		InvalidCommand(command, "Tja... niet dat het spel al is begonnen of zo...");
		return true;
	}
	

	//check players turn


	//exceute command

	return true;
}

void Game::Round(std::shared_ptr<ClientCommand> command)
{
	buildings_build = 0;
}

int Game::maxToBuildBuildings(int amount)
{
	int total = 1;

	std::vector<std::unique_ptr<Character>> &characters = current_player->Characters();

	for (auto &i : characters) {
		total = i->maxToBuildBuildings(total);
	}

	return total;
}

Game::~Game()
{
}

void Game::InvalidCommand(std::shared_ptr<ClientCommand> command, std::string value)
{
	std::shared_ptr<Socket> out = command->get_client();

	out->write("\33[31;40m" + value + "\r\nMachiavelli> ");
	out->write("\33[30;40m\33g");
}

void Game::WriteChatLine(std::pair<std::string, std::string> line)
{
	std::string finished_line = line.first + " >> " + line.second +"\r\n";

	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int col = 10;

	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, col);

	for (auto &i : players) {
		i->write(finished_line);
	}

	SetConsoleTextAttribute(hConsole, 15);
}

void Game::ClearScreen(std::shared_ptr<Player> player)
{
	player->write("\33[2J");
}
