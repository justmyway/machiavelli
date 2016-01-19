#pragma once
#include <queue>
#include <Memory>
#include <map>

#include "FileReader.h"
#include "Card.h"
#include "Character.h"
#include "Player.h"

class ClientCommand;

class Game
{
public:
	Game();

	//setup
	void FillDeckMap();
	void PrepareCharacters();
	void PrepareCards();
	bool Particatate(std::shared_ptr<Player> player);
	void Start();

	//during game
	std::vector<std::shared_ptr<Player>> Players() { return players; };
	bool Execute(std::shared_ptr<ClientCommand> command);

	void Round(std::shared_ptr<ClientCommand> command);

	//variables
	int maxToBuildBuildings(int amount);

	~Game();
private:
	std::unique_ptr<FileReader> file_reader;

	void InvalidCommand(std::shared_ptr<ClientCommand> command, std::string value);
	void WriteChat();
	void WriteChatLine(std::pair<std::string, std::string> line);

	bool started = false;
	int GoldReserve;
	std::vector<std::shared_ptr<Player>> players;
	std::queue<std::unique_ptr<Card>> deck;
	std::vector<std::unique_ptr<Card>> discard_deck;
	std::map<std::string, std::unique_ptr<Card>> deck_map;
	std::vector<std::unique_ptr<Character>> characterset;
	std::vector<std::unique_ptr<Character>> discard_characterset;
	std::map<std::string, std::unique_ptr<Character>> character_map;
	std::string deck_path = "Bouwkaarten.csv";
	std::string character_path = "karakterkaarten.csv";

	std::vector<std::pair<std::string, std::string>> chat_history;
	std::vector<std::string> valid_commands;

	//round varibale
	std::shared_ptr<Player> current_player;
	bool character_action_played;
	int buildings_build;
	bool cheatMode;

	//round functions
	void changePlayer();
	void discardcard(std::shared_ptr<Character> character);
	void ClearScreen(std::shared_ptr<Player> player);
};