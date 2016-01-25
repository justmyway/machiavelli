#include "CommandHandler.h"
#include "ClientCommand.h"

bool CommandHandler::Running()
{
	return false;
}

void CommandHandler::Excecute(ClientCommand cmd, int fase)
{
}

std::pair<std::string, std::string> CommandHandler::PrintInfo()
{
	return std::pair<std::string, std::string>();
}

int CommandHandler::AmountOfCardsToTake()
{
	return 0;
}

int CommandHandler::AmountOfCardsToreturn()
{
	return 0;
}

bool CommandHandler::CanBeBought()
{
	return false;
}

bool CommandHandler::MayBeDestroyed()
{
	return false;
}