#pragma once
#include <utility>
#include <string>

class ClientCommand;

class Command
{
public:
	//Settings
	virtual bool Running() = 0;
	virtual void Excecute(ClientCommand cmd, int fase) = 0;

	//ActionCommands
	virtual std::pair<std::string, std::string> PrintInfo() = 0;

	//RadialCommands
	virtual int AmountOfCardsToTake() = 0;
	virtual int AmountOfCardsToreturn() = 0;
	virtual bool CanBeBought() = 0;
	virtual bool MayBeDestroyed() = 0;


	virtual ~Command() = 0;
protected:
	bool running;
};

