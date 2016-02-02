#pragma once
#include <queue>
#include <deque>
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

	//Setup
	void FillDeckMap();
	void PrepareCharacters();
	void PrepareCards();
	bool Particatate(std::shared_ptr<Player> player);
	void Start();

	//During game
	bool Execute(std::shared_ptr<ClientCommand> command);
	std::shared_ptr<Player> GetOpponent(std::shared_ptr<Socket> socket);
	std::unique_ptr<Card> DrawCard();
	void DiscardCard(std::unique_ptr<Card> card);

	//Round
	std::vector<std::shared_ptr<Player>> Players() { return players; };

	~Game();
private:
	//Setup
	std::unique_ptr<FileReader> file_reader;
	std::vector<std::string> valid_commands;

	std::vector<std::shared_ptr<Player>> players;

	std::deque<std::unique_ptr<Card>> clean_deck;
	std::queue<std::unique_ptr<Card>> deck;
	std::deque<std::unique_ptr<Card>> discard_deck;
	std::map<std::string, std::unique_ptr<Card>> deck_map;

	std::vector<std::shared_ptr<Character>> characterset;
	std::deque<std::shared_ptr<Character>> choseable_characterset;
	std::vector<std::shared_ptr<Character>> discard_characterset;
	std::map<std::string, std::shared_ptr<Character>> character_map;

	std::string deck_path = "Bouwkaarten.csv";
	std::string character_path = "karakterkaarten.csv";

	bool cheat_mode;
	bool started;
	int GoldReserve;

	//During game - chat
	void ClearScreen(std::shared_ptr<Player> player);
	void InvalidCommand(std::shared_ptr<ClientCommand> command, std::string value);
	void Write(const std::string &value);
	void WriteChat();
	void WriteChatLine(const std::pair<std::string, std::string> &line);
	std::vector<std::pair<std::string, std::string>> chat_history;

	//Round varibale
	bool roundSetup;
	int order;
	std::shared_ptr<Player> start_player;
	std::shared_ptr<Player> current_player;
	
	//Round setup
	void SetupRound();
	void RemoveFirstCard();
	void ChoseCards(std::shared_ptr<ClientCommand> command, std::string cmd);
	void PickCard(int index);
	void RemoveCard(int index);
	void PrintChosableCaracters();
	void NextPlayer();
	bool YourTurn(std::shared_ptr<ClientCommand> command);

	//Round functions
	bool CallNextCharacter();
	void CallCharacter();

};