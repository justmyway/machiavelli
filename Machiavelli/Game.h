#pragma once
#include <queue>
#include <Memory>
#include <map>

#include "FileReader.h"
#include "Card.h"
#include "Character.h"


class Game
{
public:
	Game();

	void FillDeckMap();
	void PrepareCharacters();
	void PrepareCards();

	~Game();
private:
	std::unique_ptr<FileReader> file_reader;

	std::queue<std::unique_ptr<Card>> deck;
	std::vector<std::unique_ptr<Card>> discard_deck;
	std::map<std::string, std::unique_ptr<Card>> deck_map;
	std::vector<std::unique_ptr<Character>> characterset;
	std::vector<std::unique_ptr<Character>> discard_characterset;
	std::map<std::string, std::unique_ptr<Character>> character_map;
	std::string deck_path = "Bouwkaarten.csv";
	std::string character_path = "karakterkaarten.csv";
};

