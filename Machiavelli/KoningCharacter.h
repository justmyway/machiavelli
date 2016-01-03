#pragma once
#include "Character.h"

class KoningCharacter :
	public Character
{
public:
	KoningCharacter(const std::string name, int order) : Character{ name, order } {};
	static std::unique_ptr<Character> __stdcall Create(const std::string name, int order);

	CharacterColor Color();

	~KoningCharacter();
};

