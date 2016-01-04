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

	~Game();
private:
	std::unique_ptr<FileReader> file_reader;

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
};

