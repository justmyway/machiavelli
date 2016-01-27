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

	//game properties
	started = false;
	GoldReserve = 30;

	//shuffle cards
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	auto engine = std::default_random_engine{};
	std::shuffle(clean_deck.begin(), clean_deck.end(), engine);
	deck = std::queue<std::unique_ptr<Card>>(std::move(clean_deck));

	//cheatmode 
	cheatMode = false;

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

	Write("De game gaat beginnen! \r\n");

	started = true;

	//setup start
	current_player = players.front();

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
	SetupRound();
	RemoveFirstCard();
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

	//is next player, call next character
	if (cmd.compare("volgende") == 0) {
		while (!CallNextCharacter())
			;
		return true;
	}
	
	return true;
}

Game::~Game()
{
}

void Game::ClearScreen(std::shared_ptr<Player> player)
{
	player->write("\33[2J");
}

void Game::InvalidCommand(std::shared_ptr<ClientCommand> command, std::string value)
{
	std::shared_ptr<Socket> out = command->get_client();

	out->write("\33[31;40m" + value + "\r\n\x1b[37;40mMachiavelli> ");
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

	for (auto &character : characterset){
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
			current_player->write("Je kan een van de volgende kaarten kiezen en afleggen: \n");
			PrintChosableCaracters();
			return;
		}
		else {
			current_player->writeError("Dat number bestaat niet! Deze kaart kan je niet kiezen. \nProbeer het nogmaals.");
			return;
		}
	}else{
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
		CallNextCharacter();
	}
	else {
		//next player
		NextPlayer();
		current_player->write("You can chose and remove one of the folowing: \n");
		PrintChosableCaracters();
	}	
}

void Game::PickCard(int index)
{
	current_player->write("Je hebt de " + choseable_characterset.at(index)->Name() + " gekozen. \n");
	choseable_characterset.at(index)->HoldedBy(current_player);
	current_player->addCharacter(choseable_characterset.at(index));
	choseable_characterset.erase(choseable_characterset.begin() + index);
}

void Game::RemoveCard(int index)
{
	current_player->write("Je hebt de " + choseable_characterset.at(index)->Name() + " weg gestopt. \n");
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
	std::cout << players.size() << std::endl;
	if (players.at(0)->GetSocket() == socket) {
		return players.at(1);
	}
	else {
		return players.at(0);
	}
}

bool Game::CallNextCharacter()
{
	order++;
	if (order == 9) {
		SetupRound();
		return false;
	}

	if (characterset.at(order - 1)->Killed()) {
		return false;
	}

	CallCharacter();

	current_player = characterset.at(order - 1)->HoldedBy();

	if (current_player) {
		current_player->CallCaracter(characterset.at(order - 1));
		return true;
	}

	return false;
}

void Game::CallCharacter()
{
	Write("\x1b[33;1mKoning: \"Ik zou graag de " + characterset.at(order - 1)->Name() + " naar voren roepen!\"\x1b[30;40m\n");
}

/*std::vector<std::unique_ptr<Character>> Game::chooseCard(std::unique_ptr<Character>& character, std::vector<std::unique_ptr<Character>>& shuffled_characters)
{
	current_player->write("Te kiezen kaarten:");
	for (auto &card : shuffled_characters)
	{
		current_player->write(card->Name());
	}
	current_player->addCharacter(std::move(character));
	//TODO verwijder uit geschudde kaarten
	//shuffled_characters.erase(character);

	return shuffled_characters;
}

std::vector<std::unique_ptr<Character>> Game::discardCard(std::unique_ptr<Character>& character, std::vector<std::unique_ptr<Character>>& shuffled_characters)
{
	current_player->write("Te kiezen kaarten om af te leggen:");
	for (auto &card : shuffled_characters)
	{
		current_player->write(card->Name());
	}
	discard_characterset.push_back(std::move(character));
	
	//TODO verwijder uit geschudde kaarten
	//shuffled_characters.erase(character);

	return shuffled_characters;
}*/