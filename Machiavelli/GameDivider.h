#pragma once

#include "ClientCommand.h"
#include "Game.h"
#include "Player.h"

#include <vector>
#include <memory>

class GameDivider
{
public:
	GameDivider();

	void PassCommand(std::shared_ptr<ClientCommand> command);
	
	~GameDivider();
private:
	std::vector<std::unique_ptr<Game>> waiting_games;
	std::vector<std::unique_ptr<Game>> games;
	std::vector<std::shared_ptr<Player>> active_players;
	std::vector<std::shared_ptr<Player>> waiting_players;

	//Help commands
	std::vector<std::string> help_commands;
	void HelpCommand(std::shared_ptr<ClientCommand> command);
	void WriteToClient(std::shared_ptr<Socket> socket, const std::string &value);
	void WriteToClientInput(std::shared_ptr<Socket> socket);

	//Direct commands
	void JoinNewGame(std::shared_ptr<ClientCommand> command);
	void GetCommandToCorrectGame(std::shared_ptr<ClientCommand> command);

	//Move players
	void ShiftWaitingPlayer(std::shared_ptr<Player> player);
	void AddWaitingPlayer(std::shared_ptr<Player> player);
};

  