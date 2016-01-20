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
	Player(const std::string& name, const std::shared_ptr<Socket> socket) : name{ name }, client{socket}, startPlayer { false }, stash{ 0 } {}

	//Setup
	std::string get_name() const { return name; };
	void set_name(const std::string& new_name) { name = new_name; };
	void write(std::string value);
	void writeError(std::string value);

	//Who is king
	void setStartPlayer(bool start) { startPlayer = start; };
	bool StartPlayer() { return startPlayer; };

	//Printing info
	std::vector<std::string> PrintBuildings();
	std::vector<std::string> PrintCards();

	//Round
	bool NewRound(int order);

		//Fase1
		void Fase1();

		void CollectCash();
		void ChoseCards();
		
		int BuildingsWithColor(CardColor color);
		//end Fase1

	int Deposit() { return stash; };
	std::vector<std::unique_ptr<Character>> &Characters() { return character_cards; };
	void addCharacter(std::unique_ptr<Character> &Character);
	bool Build(unsigned int buildingIndex);
	
	std::vector<std::unique_ptr<Card>> ReturnCards() { return std::move(cards); };
	//end Round
	
private:
	//system to know
	std::string name;
	std::shared_ptr<Socket> client;

	//game items
	bool startPlayer;
	int stash;

	//Game Round
	//end Game Round

	//Character turn
	int maxBuildingsToBuild;
	std::unique_ptr<Character> current_character;
	//end Character turn

	std::vector<std::unique_ptr<Card>> buildings;
	std::vector<std::unique_ptr<Card>> cards;
	std::vector<std::unique_ptr<Character>> character_cards;
};

#endif /* Player_h */
