#ifndef Player_h
#define Player_h

#include "Card.h"
#include "Character.h"

#include <string>
#include <vector>
#include <memory>

class Socket;

class Player {
public:
	Player() {}
	Player(const std::string& name, const std::shared_ptr<Socket> socket) : name{ name }, client{socket}, king { false }, stash{ 0 } {}

	//Setup
	std::string get_name() const { return name; };
	void set_name(const std::string& new_name) { name = new_name; };
	void Ingame(std::shared_ptr<Game> currentGame) { game = currentGame; };
	void write(std::string value);
	void writeError(std::string value);	

	//Printing info
	std::vector<std::string> PrintBuildings();
	std::vector<std::string> PrintCards();

	//Round
	void NewRound();
	
	void King(bool start) { king = start; };
	bool King() { return king; };
	
	int Deposit() { return stash; };
	void addCharacter(std::shared_ptr<Character> Character);

		//turn
		void CallCaracter(std::shared_ptr<Character> character);

		//Fase1
		void Fase1();

		void CollectCash();
		void ChoseCards();
		
		int BuildingsWithColor(CharacterColor color);
		//end Fase1

		//Fase2
		bool Build(unsigned int buildingIndex);
		std::vector<std::unique_ptr<Card>> ReturnCards() { return std::move(cards); };

			//character properties
			void Kill(std::string &name);
			void Destroy(int index);

		//end Fase2

	//end Round
	
private:
	//Setup
	std::string name;
	std::shared_ptr<Socket> client;
	std::shared_ptr<Game> game;

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
	void ResetTurn();
	int buildingsBuild;
	//end Character turn
};

#endif /* Player_h */
