#include "GameDivider.h"
#include "Socket.h"

#include <algorithm>
#include <memory>

GameDivider::GameDivider()
{
}

void GameDivider::PassCommand(std::shared_ptr<ClientCommand> command)
{
	//player already playing?
	if (std::find(active_players.begin(), active_players.end(), command->get_player()) != active_players.end()) {
		GetCommandToCorrectGame(command);
	}
	else if (std::find(waiting_players.begin(), waiting_players.end(), command->get_player()) != waiting_players.end()) {
		command->get_client()->write("Be patient my dear the game will begin soon enough! \r\nFind a friend to play with or wait till you grow a beard. \r\n");
	}
	else {
		JoinNewGame(command);
	}
}

GameDivider::~GameDivider()
{
}

void GameDivider::JoinNewGame(std::shared_ptr<ClientCommand> command)
{
	if (waiting_games.size() > 0) {
		//join waiting game

		AddWaitingPlayer(command->get_player());
		std::unique_ptr<Game> a = std::move(waiting_games.back());
		waiting_games.pop_back();
		if (a->Particatate(command->get_player())) {
			std::vector<std::shared_ptr<Player>> game_players = a->Players();
			for (auto &game_player : game_players) {
				ShiftWaitingPlayer(game_player);
			}
			a->Start();
			games.push_back(std::move(a));
		}
		else {
			waiting_games.push_back(std::move(a));
		}
	}
	else {
		//create new game
		AddWaitingPlayer(command->get_player());
		
		std::unique_ptr<Game> new_game(new Game());
		new_game->Particatate(command->get_player());
		waiting_games.push_back(std::move(new_game));
		
	}
}

void GameDivider::GetCommandToCorrectGame(std::shared_ptr<ClientCommand> command)
{
	for (auto &i : games) {
		if (i->Execute(command))
			continue;
	}
}

void GameDivider::ShiftWaitingPlayer(std::shared_ptr<Player> player)
{
	waiting_players.erase(remove(waiting_players.begin(), waiting_players.end(), player), waiting_players.end());
	active_players.push_back(player);
}

void GameDivider::AddWaitingPlayer(std::shared_ptr<Player> player)
{
	waiting_players.push_back(player);
}
