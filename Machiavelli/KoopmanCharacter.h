#pragma once
#include "Character.h"

class KoopmanCharacter :
	public Character
{
public:
	KoopmanCharacter(const std::string name, int order) : Character{ name, order } {};
	static std::shared_ptr<Character> __stdcall Create(const std::string name, int order);

	CharacterColor Color() { return CharacterColor::GROEN; };

	int CollectCash();

	~KoopmanCharacter();
};

