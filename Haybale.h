#pragma once
#include "SDL/SDL.h"
#include <string>

struct Vector3
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

	Vector3 mHayBalePos;
	SDL_Texture* mTexture;
	SDL_Renderer* mRenderer;
	int size = 32;

};

