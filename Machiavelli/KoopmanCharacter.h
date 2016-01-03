#pragma once
#include "Character.h"

class KoopmanCharacter :
	public Character
{
public:
	KoopmanCharacter(const std::string name, int order) : Character{ name, order } {};
	static std::unique_ptr<Character> __stdcall Create(const std::string name, int order);

	CharacterColor Color();

	~KoopmanCharacter();
};

