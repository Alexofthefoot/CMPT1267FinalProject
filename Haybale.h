#pragma once
#include "SDL/SDL.h"
#include <string>

struct Vector2
{
	float x;
	float y;
};

class Haybale
{
public:
	void Initialize();
	int GetXPosition();
	int GetYPosition();
	//bool CanCowEat();

	Vector2 mHayBalePos;
	int size;

};

