#pragma once
#include "Character.h"

class PredikerCharacter :
	public Character
{
public:
	PredikerCharacter(const std::string name, int order) : Character{ name, order } {};
	static std::shared_ptr<Character> __stdcall Create(const std::string name, int order);

	CharacterColor Color() { return BLAUW; };
	
	int CollectCash();

	~PredikerCharacter();
};

