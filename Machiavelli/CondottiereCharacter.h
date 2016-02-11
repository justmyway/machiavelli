#pragma once
#include "Character.h"

class CondottiereCharacter :
	public Character
{
public:
	CondottiereCharacter(const std::string name, int order) : Character{ name, order } {};
	static std::shared_ptr<Character> __stdcall Create(const std::string name, int order);

	CharacterColor Color() { return CharacterColor::ROOD; };
	
	int CollectCash();

	~CondottiereCharacter();
};

