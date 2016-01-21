#pragma once
#include "Character.h"

class StandardCharacter :
	public Character
{
public:
	StandardCharacter(const std::string name, int order) : Character{ name, order } {};
	
	CharacterColor Color() { return CharacterColor::NONE; };
	
	~StandardCharacter();
};

