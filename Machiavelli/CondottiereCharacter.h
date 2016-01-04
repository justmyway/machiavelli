#pragma once
#include "Character.h"

class CondottiereCharacter :
	public Character
{
public:
	CondottiereCharacter(const std::string name, int order) : Character{ name, order } {};
	static std::unique_ptr<Character> __stdcall Create(const std::string name, int order);

	CharacterColor Color() { return ROOD; };

	~CondottiereCharacter();
};

