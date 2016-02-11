#ifndef Player_h
#define Player_h

#include "Card.h"
#include "Character.h"

#include <string>
#include <vector>
#include <memory>

class Socket;
class Game;

class Player {
public:
	Player() {}
	Player(const std::string& name, const std::shared_ptr<Socket> socket) : name{ name }, client{ socket }, king{ false }, stash{ 0 }, finish_points{0} {}

	//Setup
	std::string get_name() const { return name; };
	void set_name(const std::string& new_name) { name = new_name; };
	void Ingame(std::shared_ptr<Game> currentGame) { game = currentGame; };
	void write(std::string value, bool start = true);
	void writeError(std::string value);
	std::shared_ptr<Socket> GetSocket() { return client; };
	int CalculatePoints();

	//Printing info
	void PrintOverview();
	std::vector<std::string> PrintOverviewOpponent();
	std::vector<std::string> PrintBuildings();
	std::vector<std::string> PrintCards();
	int Buildings() { return buildings.size(); };

	//Round
	void NewRound();
	
	void King(bool start) { king = start; };
	bool King() { return king; };
	
	int Deposit() { return stash; };
	void Deposit(int amount) { stash += amount; };
	void addCharacter(std::shared_ptr<Character> character) { character_cards.push_back(character); };
	void addCard(std::unique_ptr<Card> card) { cards.push_back(std::move(card)); };
	std::vector<std::unique_ptr<Card>> getCards() { return std::move(cards); };

		//turn
		void CallCaracter(std::shared_ptr<Character> character);
		int fase;

		bool CollectCash();
		void ChoseCards();
		void ChoseCards(std::string value);
		bool Busy() { return chosing_cards; };
		
		int BuildingsWithColor(CharacterColor color);
		//end Fase1

		//Fase2
		bool Build(unsigned int buildingIndex);
		std::vector<std::unique_ptr<Card>> ReturnCards() { return std::move(cards); };

			//character properties
			void Murder(std::string &name);
			void Rob(std::string &name);
			void SwapCards(std::string &name);
			void TakeTwoCards();
			void Destroy(int index);
			bool DestroyBuilding(int index);

		//end Fase2

	//end Round
	
private:
	//Setup
	std::string name;
	std::shared_ptr<Socket> client;
	std::shared_ptr<Game> game;
	int finish_points;

	//Game
	std::vector<std::unique_ptr<Card>> buildings;
	std::vector<std::unique_ptr<Card>> cards;
	std::vector<std::shared_ptr<Character>> character_cards;

	bool king;
	int stash;

	//Game Round
	//end Game Round

	//Character turn
	std::shared_ptr<Character> current_character;
	std::vector<std::unique_ptr<Card>> drawn_cards;
	bool chosing_cards;
	void ResetTurn();
	int buildingsBuild;
	int cardsTaken;
	//end Character turn
};

#endif /* Player_h */
