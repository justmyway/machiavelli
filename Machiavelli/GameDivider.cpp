#include "GameDivider.h"
#include "Socket.h"

#include <algorithm>
#include <memory>

GameDivider::GameDivider()
{
	//Help commands
	help_commands.push_back("'overzicht'           Laat zien wat er op dit moment in het spel aanwezig is.");
	help_commands.push_back("'bouw <nummer>'       Plaats een gebouw op het speelveld. De nummers corresponderen met nummers voor de handkaarten.");
	help_commands.push_back("'vermoord <karakter>' Karakter kan een van de volgende zijn: Dief, Magier, Koning, Prediker, Koopman, Bouwmeester of Condottiere."); 
	help_commands.push_back("'besteel <karakter>'  Karakter kan een van de volgende zijn: Magier, Koning, Prediker, Koopman, Bouwmeester of Condottiere mist deze niet vermoord is)");
	help_commands.push_back("'ruil <optie>'        ruilt al jouw eigen kaarten met de tegenstander of de voorraad op het bord.");
	help_commands.push_back("'vernietig <nummer>'  Vernietig een gebouw van de tegenstander. De nummers corresponderen met de nummers voor de gebouwen van de tegenstander.");
	help_commands.push_back("'next'				   De beurt word aan de volgende speler overgedragen.");
	help_commands.push_back("'chat <tekst>'        Om te chatten typ chat gevolgd door hetgeen wat je wil zeggen");
}

void GameDivider::PassCommand(std::shared_ptr<ClientCommand> command)
{
	//help command
	if (command->get_cmd().compare("?") == 0 || command->get_cmd().compare("help") == 0) {
		HelpCommand(command);
		return;
	}

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

void GameDivider::HelpCommand(std::shared_ptr<ClientCommand> command)
{
	std::shared_ptr<Socket> socket = command->get_client();

	WriteToClient(socket, "\r==================================================================================================================================================================");
	WriteToClient(socket, "| Commando's die je kunt uitvoeren:\r|\n");

	for (auto &i : help_commands) {
		WriteToClient(socket,"| " + i);
	}

	WriteToClient(socket, "==================================================================================================================================================================");
	WriteToClientInput(socket);
}

void GameDivider::WriteToClient(std::shared_ptr<Socket> socket, const std::string &value)
{
	socket->write(value + "\r\n");
}

void GameDivider::WriteToClientInput(std::shared_ptr<Socket> socket)
{
	socket->write("machiavelli> ");
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
