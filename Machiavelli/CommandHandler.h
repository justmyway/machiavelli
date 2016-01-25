#pragma once
#include "Command.h"

class CommandHandler :
	public Command
{
public:
	//Settings
	bool Running() override;
	void Excecute(ClientCommand cmd, int fase) override;

	//ActionCommands
	std::pair<std::string, std::string> PrintInfo() override;

	//RadialCommands
	int AmountOfCardsToTake() override;
	int AmountOfCardsToreturn() override;
	bool CanBeBought() override;
	bool MayBeDestroyed() override;
};

