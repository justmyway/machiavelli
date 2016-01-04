#ifndef Player_h
#define Player_h

#include "Card.h"

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

	//Who is king
	void setStartPlayer(bool start) { startPlayer = start; };
	bool StartPlayer() { return startPlayer; };

	//Printing cards
	std::vector<std::string> PrintBuildings();
	std::vector<std::string> PrintCards();

	//Gold
	int Deposit() { return stash; };

	std::vector<std::unique_ptr<Card>> ReturnCards() { return std::move(cards); };
	
private:
	//system to know
	std::string name;
	std::shared_ptr<Socket> client;

	//game items
	bool startPlayer;
	int stash;

	std::vector<std::unique_ptr<Card>> buildings;
	std::vector<std::unique_ptr<Card>> cards;
};

#endif /* Player_h */
