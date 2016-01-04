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

	void JoinNewGame(std::shared_ptr<ClientCommand> command);
	void GetCommandToCorrectGame(std::shared_ptr<ClientCommand> command);

	void ShiftWaitingPlayer(std::shared_ptr<Player> player);
	void AddWaitingPlayer(std::shared_ptr<Player> player);
};

  