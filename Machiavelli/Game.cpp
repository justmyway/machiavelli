#include "Game.h"

#include "StandardCharacter.h"
#include "CondottiereCharacter.h"
#include "KoningCharacter.h"
#include "KoopmanCharacter.h"
#include "PredikerCharacter.h"

#include "StandardCard.h"
#include "HofDerWonderenCard.h"
#include "KerkerCard.h"
#include "KerkhofCard.h"
#include "LaboratoriumCard.h"
#include "WerkplaatsCard.h"
#include "ObservatoriumCard.h"
#include "BibliotheekCard.h"
#include "MagiersCard.h"
#include "PrestigieusGebouwCard.h"

#include <iostream>
#include <exception>
#include <iostream>

Game::Game() :
	file_reader(new FileReader())
{
	//fill character map
	character_map["Condottiere"]	= CondottiereCharacter::Create("Condottiere", 8);
	character_map["Koning"]			= KoningCharacter::Create("Koning", 4);
	character_map["Koopman"]		= KoopmanCharacter::Create("Koopman", 6);
	character_map["Prediker"]		= PredikerCharacter::Create("Prediker", 5);

	//fill deck map
	FillDeckMap();
	


	//get characters
	PrepareCharacters();

	//get cards
	PrepareCards();
}


void Game::FillDeckMap()
{
	deck_map["Hof der Wonderen"] = HofDerWonderenCard::Create();
	deck_map["Kerker"] = KerkerCard::Create();
	deck_map["Kerkhof"] = KerkhofCard::Create();
	deck_map["Laboratorium"] = LaboratoriumCard::Create();
	deck_map["Werkplaats"] = WerkplaatsCard::Create();
	deck_map["Observatorium"] = ObservatoriumCard::Create();
	deck_map["Bibliotheek"] = BibliotheekCard::Create();
	deck_map["School voor magiërs"] = MagiersCard::Create();
	deck_map["Drakenpoort"] = PrestigieusGebouwCard::Create();
	deck_map["Universiteit"] = PrestigieusGebouwCard::Create();
}

void Game::PrepareCharacters()
{
	//create all characters based on file
	std::vector<std::string> character_lines = file_reader->Read(character_path);

	for (auto &line : character_lines) {

		if (line == FileReader::_EOF)
			continue;

		std::string &name = file_reader->ColumnValue(line, 2);
		const int order = std::stoi(file_reader->ColumnValue(line, 1));

		auto it = character_map.find(name);
		if (it != character_map.end()) {
			std::unique_ptr<Character> character(std::move(it->second));
			characterset.push_back(std::move(character));
		}
		else 
		{
			std::unique_ptr<Character> character(new StandardCharacter(name, order));
			characterset.push_back(std::move(character));
		}
	}

	//check
	
	/*for (auto const& i : characterset) {
		std::cout << i->Name() << " - " << i->Color() << std::endl;
	};*/
	std::cout << "All " << characterset.size() << " characters have been made. \n";
	
	//end check
}

void Game::PrepareCards()
{
	//create all cards based on file
	std::vector<std::string> card_lines = file_reader->Read(deck_path);

	try {
		for (auto &line : card_lines) {

			if (line == FileReader::_EOF)
				continue;

			std::string &name = file_reader->ColumnValue(line, 1);
			const int cost = std::stoi(file_reader->ColumnValue(line, 2));
			std::string &color = file_reader->ColumnValue(line, 3);

			auto it = deck_map.find(name);
			if (it != deck_map.end()) {
				if (it->second == nullptr) {
					FillDeckMap();
					it = deck_map.find(name);
				}
				std::unique_ptr<Card> card(std::move(it->second));
				card->setProperties(name, cost, color, file_reader->ColumnValue(line, 4));
				deck.push(std::move(card));
			}
			else
			{
				std::unique_ptr<Card> card(new StandardCard());
				card->setProperties(name, cost, color);
				deck.push(std::move(card));
			}
		}

	}catch (const std::exception& ex) {
		std::cerr << "***************************************\n";
		std::cerr << ex.what();
		std::cerr << "***************************************\n";
	}

	//check
	
	std::cout << "All " << deck.size() << " cards have been made. \n";
	
	//end check
}

Game::~Game()
{
}
